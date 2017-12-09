#include <QCoreApplication>

#include <QBuffer>
#include <QDebug>
#include <qcountpipe.h>
#include <qhashpipe.h>
#include <qpipedevice.h>
#include <qteepipe.h>

class SamplePipe : public QPipeDevice
{
protected:
	inline QByteArray process(QByteArray &&data) override {
		return data.replace('o', '0');
	}
};

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QBuffer teeBuffer;

	QPipeDevice pipe;
	QTeePipe tee(&teeBuffer);
	QHashPipe hashPipe(QCryptographicHash::Sha3_256);
	QCountPipe countPipe;
	SamplePipe samplePipe;
	QBuffer outBuffer;

	pipe | tee | hashPipe | countPipe | samplePipe | outBuffer;

	pipe.open();
	pipe.write("Hello World!");
	pipe.close();

	qDebug() << "out buffer:" << outBuffer.isOpen() << outBuffer.data();
	qDebug() << "tee buffer:" << teeBuffer.isOpen() << teeBuffer.data();
	qDebug() << "SHA3_256:" << hashPipe.hash().toHex();
	qDebug() << "Byte Count:" << countPipe.count();

	return 0;
}
