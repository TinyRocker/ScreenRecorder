#include "XVideoWriter.h"

#include <iostream>

using namespace std;

int main(void)
{
    const char *rgbfile = "test.rgb";
    const char *pcmfile = "test.pcm";
    const char *outfile = "rgba_pcm.mp4";
    XVideoWriter *xw = XVideoWriter::getInstance(0);
    cout << "open " << outfile << " result:" << xw->open(outfile) << endl;
    cout << "add video stream:" << xw->addVideoStream() << endl;
    cout << "add audio stream:" << xw->addAudioStream() << endl;

    FILE *fp_video = nullptr;
    fopen_s(&fp_video, rgbfile, "rb");
    if (!fp_video)
    {
        cout << "fopen " << rgbfile << " failed" << endl;
        getchar();
        return -1;
    }
    
    FILE *fp_audio = nullptr;
    fopen_s(&fp_audio, pcmfile , "rb");
    if (!fp_audio)
    {
        cout << "fopen " << pcmfile << " failed" << endl;
        getchar();
        return -1;
    }

    int videoSize = xw->inWidth() * xw->inHeight() * 4; // BGRA
    uint8_t *rgb = new uint8_t[videoSize];
    
    int audioSize = xw->nbSample() * xw->inChannel() * 2;   // 16 bit == 2 byte, À˘“‘≥À2
    uint8_t *pcm = new uint8_t[audioSize];

    // –¥»ÎÕ∑≤ø
    xw->writeHead();

    int len = 0;
    AVPacket *packet = nullptr;
    while (true)
    {
        if (xw->isVideoFront())
        {
            len = fread(rgb, 1, videoSize, fp_video);
            if (len <= 0) { break; }

            packet = xw->encodeVideo(rgb);  // ±‡¬Î ”∆µ÷°
            if (xw->writeFrame(packet))     // –¥»Î ”∆µ÷°
            {
                cout << '-';
            }
        }
        else
        {
            len = fread(pcm, 1, audioSize, fp_audio);
            if (len <= 0) { break; }
            packet = xw->encodeAudio(pcm);  // ±‡¬Î“Ù∆µ÷°
            if (xw->writeFrame(packet))     // –¥»Î“Ù∆µ÷°
            {
                cout << ".";
            }
        }

        
    }
    cout << endl;

    // –¥»ÎŒ≤≤ø
    xw->writeTail();

    xw->close();

    delete[] rgb;
    rgb = nullptr;
    delete[] pcm;
    pcm = nullptr;
    fclose(fp_video);
    fclose(fp_audio);

    system("pause");
    return 0;
}