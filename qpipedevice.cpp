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

void QPipeDevice::pipeTo(QPipeDevice *pipeSink)
{
	if(_sinkPipe) {
		_sinkPipe->_sourcePipe = nullptr;
		_sinkPipe = nullptr;
	}

	if(_sink) //reset sink, if needed
		setSink(_sink);

	if(pipeSink) {
		_sinkPipe = pipeSink;
		_sinkPipe->_sourcePipe = this;
	}
}

void QPipeDevice::pipeFrom(QPipeDevice *pipeSource)
{
	if(_sourcePipe)
		_sourcePipe->pipeTo(nullptr);//will reset this as well
	if(_source)
		setSource(nullptr);//reset source, if needed
	if(pipeSource)
		pipeSource->pipeTo(this);//will setup this as well
}

QPipeDevice &QPipeDevice::operator|(QPipeDevice &other)
{
	pipeTo(&other);
	return other;
}

void QPipeDevice::operator|(QIODevice *device)
{
	setSink(device);
}

QPipeDevice &operator|(QIODevice *device, QPipeDevice &pipe)
{
	pipe.setSource(device);
	return pipe;
}

QIODevice *QPipeDevice::source() const
{
	return _source;
}

QIODevice *QPipeDevice::sink() const
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

void QPipeDevice::setSource(QIODevice *source)
{
	if(_source) {
		_source->disconnect(this);
		_source = nullptr;
	}

	if(_sourcePipe)
		pipeFrom(nullptr);

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
}

void QPipeDevice::setSink(QIODevice *sink)
{
	if(_sink) {
		_sink->disconnect(this);
		_sink = nullptr;
	}

	if(_sinkPipe)
		pipeTo(nullptr);

	if(sink)
		_sink = sink;
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
