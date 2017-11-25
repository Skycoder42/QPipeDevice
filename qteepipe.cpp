#include "qteepipe.h"

QTeePipe::QTeePipe(QObject *parent) :
	QPipeDevice(parent)
{}

QIODevice *QTeePipe::teeDevice() const
{
	return _teeDevice;
}

void QTeePipe::setTeeDevice(QIODevice *teeDevice)
{
	_teeDevice = teeDevice;
}

QByteArray QTeePipe::process(QByteArray data)
{
	if(_teeDevice && _teeDevice->isWritable())
		_teeDevice->write(data);
	return data;
}

void QTeePipe::end()
{
	emit finished();
	if(_teeDevice && autoClose() && _teeDevice->isOpen())
		_teeDevice->close();
}
