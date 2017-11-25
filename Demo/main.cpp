#include <QCoreApplication>

#include <QBuffer>
#include <QDebug>
#include <qhashpipe.h>
#include <qpipedevice.h>
#include <qteepipe.h>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QBuffer inBuffer;
	inBuffer.setData("Hello World");
	inBuffer.open(QIODevice::ReadOnly);

	QBuffer outBuffer;
	outBuffer.open(QIODevice::WriteOnly);

	QPipeDevice pipe;
	pipe.setAutoClose(true);

	QBuffer teeBuffer;
	teeBuffer.open(QIODevice::WriteOnly);
	QTeePipe tee;
	tee.setAutoClose(true);
	tee.setTeeDevice(&teeBuffer);

	QHashPipe hashPipe(QCryptographicHash::Sha3_256);

	inBuffer | pipe | tee | hashPipe | outBuffer;
	pipe.flush();
	inBuffer.close();

	qDebug() << "out buffer" << outBuffer.isOpen() << outBuffer.data();
	qDebug() << "tee buffer" << teeBuffer.isOpen() << teeBuffer.data();
	qDebug() << "SHA3_256" << hashPipe.hash().toHex();

	return 0;
}
