#include <QAudioInput>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QAudioFormat fmt;
    fmt.setSampleRate(44100);
    fmt.setSampleSize(16);
    fmt.setChannelCount(2);
    fmt.setSampleType(QAudioFormat::UnSignedInt);
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    fmt.setCodec("audio/pcm");

    FILE *fp = nullptr;
    fopen_s(&fp, "test_out.pcm", "wb");

    QAudioInput *input = new QAudioInput(fmt);
    QIODevice *io = input->start();
    char *buf = new char[1024];
    int br = 0;
    int len = 0;
    int total = 0;
    while (true)
    {
        br = input->bytesReady();
        if (br < 1024) continue;
        len = io->read(buf, 1024);
        fwrite(buf, 1, len, fp);
        cout << len << '|';
        total += len;

        if (total > 2 * 1024 * 1024) break;
    }

    fclose(fp);

    system("pause");
    return 0;
}
