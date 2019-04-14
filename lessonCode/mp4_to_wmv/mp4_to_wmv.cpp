#include <iostream>
using namespace std;

extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/error.h"
}

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")

char *errStr(int err)
{
    char str[512] = { 0 };
    av_strerror(err, str, 512);
    return str;
}

int main(void)
{
    const char *infile = "../../testFile/big_buck_bunny_720p_30mb.mp4";
    const char *outfile = "testout.flv";
    int ret;
    // 注册所有的封装器 muxer，解封器 demuters
    //av_register_all();

    // step 1:open input file
    AVFormatContext *ic = nullptr;
    ret = avformat_open_input(&ic, infile, nullptr, nullptr);
    if (!ic)
    {
        cout << "avformat_open_input failed! reason:" << errStr(ret) << endl;
        getchar();
        return -1;
    }
    cout << "open " << infile << " success!" << endl;

    // step 2:create output context
    AVFormatContext *oc = nullptr;
    ret = avformat_alloc_output_context2(&oc, nullptr, nullptr, outfile);
    if (!oc)
    {
        cout << "avformat_alloc_output_context2 failed! reason:" << errStr(ret) << endl;
        getchar();
        return -1;
    }

    // step3:add the stream
    AVStream *videoStream = avformat_new_stream(oc, nullptr);
    AVStream *audioStream = avformat_new_stream(oc, nullptr);

    // step4:copy stream
    avcodec_parameters_copy(videoStream->codecpar, ic->streams[AVMEDIA_TYPE_VIDEO]->codecpar);
    avcodec_parameters_copy(audioStream->codecpar, ic->streams[AVMEDIA_TYPE_AUDIO]->codecpar);

    // 设置为0，不做编码
    videoStream->codecpar->codec_tag = 0;
    audioStream->codecpar->codec_tag = 0;

    //av_dump_format(ic, AVMEDIA_TYPE_AUDIO, infile, 0);
    av_dump_format(ic, AVMEDIA_TYPE_VIDEO, infile, 0);
    cout << "================================================================" << endl;
    //av_dump_format(oc, AVMEDIA_TYPE_AUDIO, outfile, 1);
    av_dump_format(oc, AVMEDIA_TYPE_VIDEO, outfile, 1);

    // step 5:open out file io, write head
    ret = avio_open(&oc->pb, outfile, AVIO_FLAG_WRITE);
    if (ret < 0)
    {
        cout << "avio_open failed! reason:" << errStr(ret) << endl;
        getchar();
        return -1;
    }
    ret = avformat_write_header(oc, nullptr);
    if (ret < 0)
    {
        cout << "avformat_write_header failed! reason:" << errStr(ret) << endl;
        getchar();
        return -1;
    }

    // step 6:read frame
    AVPacket packet;
    while (true)
    {
        ret = av_read_frame(ic, &packet);   // input的ic中 读一帧
        if (ret < 0) { break; };
        // 计算packet显示时间
        packet.pts = av_rescale_q_rnd(packet.pts,
            ic->streams[packet.stream_index]->time_base,
            oc->streams[packet.stream_index]->time_base,
            (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        // 计算packet解码时间
        packet.dts = av_rescale_q_rnd(packet.dts,
            ic->streams[packet.stream_index]->time_base,
            oc->streams[packet.stream_index]->time_base,
            (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));

        packet.pos = -1;    // 在文件当中的位置
        // 计算packet持续时间
        packet.duration = av_rescale_q_rnd(packet.duration,
            ic->streams[packet.stream_index]->time_base,
            oc->streams[packet.stream_index]->time_base,
            (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));

        av_write_frame(oc, &packet);        // output的oc中 写一帧
        av_packet_unref(&packet);           // packet置空
        cout << "*";
    }
    cout << endl;
    cout << "==================end========================" << endl;

    // step 7:read tail
    ret = av_write_trailer(oc);
    if (ret != 0)
    {
        cout << "av_write_trailer failed! reason:" << errStr(ret) << endl;
        getchar();
        return -1;
    }
    
    // step 8:close out file
    avio_close(oc->pb);
    
    system("pause");
    return 0;
}