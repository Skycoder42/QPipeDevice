#include "qcountpipe.h"

QCountPipe::QCountPipe(QObject *parent) :
	QPipeDevice(parent),
	_count(0)
{}

qint64 QCountPipe::count() const
{
	return _count;
}

void QCountPipe::resetCount()
{
	_count = 0;
}

QByteArray QCountPipe::process(QByteArray data)
{
	_count += (qint64)data.size();
	return data;
}
