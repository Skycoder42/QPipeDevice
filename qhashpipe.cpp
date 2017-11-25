#include "qhashpipe.h"

QHashPipe::QHashPipe(QCryptographicHash::Algorithm algorithm, QObject *parent) :
	QPipeDevice(parent),
	_hash(algorithm)
{}

QByteArray QHashPipe::hash() const
{
	return _hash.result();
}

void QHashPipe::reset()
{
	_hash.reset();
}

QByteArray QHashPipe::process(QByteArray data)
{
	_hash.addData(data);
	return data;
}
