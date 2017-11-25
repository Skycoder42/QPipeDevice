#include "qpipedevice.h"

QPipeDevice::QPipeDevice(QObject *parent) :
	QObject(parent),
	_source(nullptr),
	_sourcePipe(nullptr),
	_sink(nullptr),
	_sinkPipe(nullptr),
	_blockSize(1ll * 1024ll * 1024ll), //1MB blocksize
	_autoClose(false)
{}

QIODevice *QPipeDevice::sourceDevice() const
{
	return _source;
}

QIODevice *QPipeDevice::sinkDevice() const
{
	return _sink;
}

QPipeDevice *QPipeDevice::sourcePipe() const
{
	return _sourcePipe;
}

QPipeDevice *QPipeDevice::sinkPipe() const
{
	return _sinkPipe;
}

qint64 QPipeDevice::blockSize() const
{
	return _blockSize;
}

bool QPipeDevice::autoClose() const
{
	return _autoClose;
}

QPipeDevice *QPipeDevice::pipeTo(QPipeDevice *sink)
{
	if(_sinkPipe) {
		_sinkPipe->_sourcePipe = nullptr;
		_sinkPipe = nullptr;
	}

	if(_sink) //reset sink, if needed
		pipeTo((QIODevice*)nullptr);

	if(sink) {
		_sinkPipe = sink;
		_sinkPipe->_sourcePipe = this;
	}

	return sink;
}

void QPipeDevice::pipeTo(QIODevice *sink)
{
	if(_sink)
		_sink = nullptr;

	if(_sinkPipe)
		pipeTo((QPipeDevice*)nullptr);

	if(sink)
		_sink = sink;
}

QPipeDevice *QPipeDevice::pipeFrom(QPipeDevice *source)
{
	if(_sourcePipe)
		_sourcePipe->pipeTo((QPipeDevice*)nullptr);//will reset this as well
	if(_source)
		pipeFrom((QIODevice*)nullptr);//reset source, if needed
	if(source)
		source->pipeTo(this);//will setup this as well

	return this;
}

QPipeDevice *QPipeDevice::pipeFrom(QIODevice *source)
{
	if(_source) {
		_source->disconnect(this);
		_source = nullptr;
	}

	if(_sourcePipe)
		pipeFrom((QPipeDevice*)nullptr);

	if(source) {
		_source = source;

		connect(_source, &QIODevice::readyRead,
				this, &QPipeDevice::readyRead);
		connect(_source, &QIODevice::readChannelFinished,
				this, &QPipeDevice::readClosed);
		connect(_source, &QIODevice::aboutToClose,
				this, &QPipeDevice::close);

		if(_source->isReadable() && _source->bytesAvailable() > 0ll)
			QMetaObject::invokeMethod(this, "readyRead", Qt::QueuedConnection);
	}

	return this;
}

void QPipeDevice::setBlockSize(qint64 blockSize)
{
	_blockSize = blockSize;
}

void QPipeDevice::resetBlockSize()
{
	setBlockSize(1ll * 1024ll * 1024ll);
}

void QPipeDevice::setAutoClose(bool autoClose)
{
	_autoClose = autoClose;
}

QByteArray QPipeDevice::process(QByteArray data)
{
	return data;
}

void QPipeDevice::end() {}

void QPipeDevice::readyRead()
{
	if(!_source)
		return;

	if(_blockSize == 0)
		pipeData(_source->readAll());
	else {
		qint64 count = 0;
		while((count = _source->bytesAvailable()) > 0) {
			count = qMin(count, _blockSize);
			pipeData(_source->read(count));
		}
	}
}

void QPipeDevice::readClosed()
{
	end();
	if(_sinkPipe)
		_sinkPipe->readClosed();
}

void QPipeDevice::close()
{
	if(_sinkPipe)
		_sinkPipe->close();
	else if(_sink && _autoClose)
		_sink->close();
}

void QPipeDevice::pipeData(const QByteArray &data)
{
	if(_sinkPipe)
		_sinkPipe->pipeData(process(data));
	else if(_sink)
		_sink->write(process(data));
}



QPointer<QPipeDevice> operator|(QPointer<QPipeDevice> source, QPointer<QPipeDevice> sink)
{
	return source->pipeTo(sink);
}

void operator|(QPointer<QPipeDevice> source, QPointer<QIODevice> sink)
{
	return source->pipeTo(sink);
}

QPointer<QPipeDevice> operator|(QPointer<QIODevice> source, QPointer<QPipeDevice> sink)
{
	return sink->pipeFrom(source);
}
