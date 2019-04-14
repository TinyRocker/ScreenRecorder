#include <iostream>
using namespace std;

extern "C"
{
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
#include "libavutil/error.h"
}

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")

char *errStr(int err)
{
    char str[512] = { 0 };
    av_strerror(err, str, 512);
    return str;
}

int main(void)
{
    //const char *infile = "../../testFile/hz44100_2ch_320kbps.wav";
    const char *infile = "test_out.pcm";
    const char *outfile = "testout.aac";
    int ret;

    // step 1:打开音频编码器
    AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    if (!codec)
    {
        cout << "avcodec_find_encoder failed" << endl;
        getchar();
        return -1;
    }
    AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
    {
        if (!codecCtx)
        {
            cout << "avcodec_alloc_context3 failed" << endl;
            getchar();
            return -1;
        }
    }
    codecCtx->bit_rate = 64000;
    //codecCtx->bit_rate = 320000;
    codecCtx->sample_rate = 44100;
    codecCtx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    codecCtx->channel_layout = AV_CH_LAYOUT_STEREO;
    codecCtx->channels = 2;
    codecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    ret = avcodec_open2(codecCtx, codec, nullptr);
    if (ret < 0)
    {
        cout << "avcodec_open2 failed, reason:" << errStr(ret) << endl;
        getchar();
        return -1;
    }
    cout << "avcodec_open2 success!" << endl;

    // step 2:打开输出封装的上下文
    AVFormatContext *oc = nullptr;
    ret = avformat_alloc_output_context2(&oc, nullptr, nullptr, outfile);
    if (ret < 0)
    {
        cout << "avformat_alloc_output_context2 failed, reason:" << errStr(ret) << endl;
        getchar();
        return -1;
    }

    AVStream *audioStream = avformat_new_stream(oc, nullptr);
    audioStream->codecpar->codec_tag = 0;
    avcodec_parameters_from_context(audioStream->codecpar, codecCtx);
    
    av_dump_format(oc, 0, outfile, 1);
    cout << "========================end===========================" << endl;

    // step 3: open io, write head
    ret = avio_open(&oc->pb, outfile, AVIO_FLAG_WRITE);
    if (ret < 0)
    {
        cout << "avio_open failed, reason:" << errStr(ret) << endl;
        getchar();
        return -1;
    }

    ret = avformat_write_header(oc, nullptr);
    if (ret < 0)
    {
        cout << "avformat_write_header failed, reason:" << errStr(ret) << endl;
        getchar();
        return -1;
    }

    // 44100 16bit 2ch
    // step 4: 创建音频重采样上下文
    SwrContext *audioCtx = nullptr;
    audioCtx = swr_alloc_set_opts(audioCtx,
        codecCtx->channel_layout, codecCtx->sample_fmt, codecCtx->sample_rate,  // 输出格式
        AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100,                          // 输入格式
        0, nullptr);
    if (!audioCtx)
    {
        cout << "swr_alloc_set_opts failed" << endl;
        getchar();
        return -1;
    }

    ret = swr_init(audioCtx);
    if (ret < 0)
    {
        cout << "swr_init failed, reason:" << errStr(ret) << endl;
        getchar();
        return -1;
    }

    // step 5: 打开输入音频文件进行重采样
    AVFrame *frame = av_frame_alloc();
    frame->format = AV_SAMPLE_FMT_FLTP;
    frame->channels = 2;
    frame->channel_layout = AV_CH_LAYOUT_STEREO;
    frame->nb_samples = 1024;        // 一帧音频存放的样本数量
    ret = av_frame_get_buffer(frame, 0);
    if (ret < 0)
    {
        cout << "av_frame_get_buffer failed, reason:" << errStr(ret) << endl;
        getchar();
        return -1;
    }

    // 16bit == 2 byte, 2 channel
    int readSize = frame->nb_samples * 2 * 2;
    uint8_t *pcm = new uint8_t[readSize];
    int len = 0;
    FILE *fp = nullptr;
    fopen_s(&fp, infile, "rb");
    if (!fp)
    {
        cout << "fopen_s " << infile << " failed" << endl;
        getchar();
        return -1;
    }

    const uint8_t *data[1] = { 0 };
    while (true)
    {
        len = fread(pcm, 1, readSize, fp);
        if (len <= 0) { break; }
        
        data[0] = (uint8_t *)pcm;
        len = swr_convert(audioCtx, frame->data, frame->nb_samples, data, frame->nb_samples);
        if (len <= 0) { break; }

        cout << '[' << len << ']';

        // step 6: 音频编码
        AVPacket packet;
        av_init_packet(&packet);
        ret = avcodec_send_frame(codecCtx, frame);
        if (ret != 0)
        {
            cout << "avcodec_send_frame failed, reason:" << errStr(ret) << endl;
            continue;
        }
        ret = avcodec_receive_packet(codecCtx, &packet);
        if (ret != 0)
        {
            cout << "avcodec_receive_packet failed, reason:" << errStr(ret) << endl;
            continue;
        }

        // step 7: 音频封装，如aac文件
        packet.stream_index = 0;
        packet.pts = 0;
        packet.dts = 0;

        ret = av_interleaved_write_frame(oc, &packet);
    }

    av_write_trailer(oc);   // 写入尾部（索引）

    avio_close(oc->pb);     // 关闭输出io

    avformat_free_context(oc);  // 清理封装输出上下文

    avcodec_close(codecCtx);    // 关闭编码器

    avcodec_free_context(&codecCtx);    // 清理重采样上下文

    system("pause");
    return 0;
}