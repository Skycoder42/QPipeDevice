#include "qteepipe.h"

QTeePipe::QTeePipe(QIODevice *teeDevice, QObject *parent) :
	QPipeDevice(parent),
	_teeDevice(teeDevice)
{}

QTeePipe::QTeePipe(QObject *parent) :
	QTeePipe(nullptr, parent)
{}

QIODevice *QTeePipe::teeDevice() const
{
	return _teeDevice;
}

void QTeePipe::setTeeDevice(QIODevice *teeDevice)
{
	_teeDevice = teeDevice;
}

void QTeePipe::init(bool buffered)
{
	if(_teeDevice && autoOpen() && !_teeDevice->isOpen())
		_teeDevice->open(WriteOnly | (buffered ? NotOpen : Unbuffered));
}

QByteArray QTeePipe::process(QByteArray &&data)
{
	if(_teeDevice && _teeDevice->isWritable())
		_teeDevice->write(data);
	return data;
}

void QTeePipe::end()
{
	if(_teeDevice && autoClose() && _teeDevice->isOpen())
		_teeDevice->close();
}
