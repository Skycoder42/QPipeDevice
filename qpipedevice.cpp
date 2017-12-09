#include "qpipedevice.h"

QPipeDevice::QPipeDevice(QObject *parent) :
	QIODevice(parent),
	_sink(nullptr),
	_autoOpen(true),
	_autoClose(true)
{}

QIODevice *QPipeDevice::sink() const
{
	return _sink;
}

bool QPipeDevice::autoOpen() const
{
	return _autoOpen;
}

bool QPipeDevice::autoClose() const
{
	return _autoClose;
}

QPipeDevice *QPipeDevice::pipeTo(QPipeDevice *sink)
{
	_sink = sink;
	return sink;
}

void QPipeDevice::pipeTo(QIODevice *sink)
{
	_sink = sink;
}

bool QPipeDevice::isSequential() const
{
	return true;
}

bool QPipeDevice::open(bool buffered)
{
	if(!_sink) {
		setErrorString(tr("Cannot open pipe without setting a sink device"));
		return false;
	}

	auto mMode = WriteOnly | (buffered ? NotOpen : Unbuffered);
	if(QIODevice::open(mMode)) {
		init(buffered);
		if(_autoOpen)
			return _sink->open(mMode);
		else
			return true;
	} else
		return false;
}

void QPipeDevice::close()
{
	end();
	QIODevice::close();
	if(_autoClose && _sink)
		_sink->close();
}

void QPipeDevice::setAutoOpen(bool autoOpen)
{
	_autoOpen = autoOpen;
}

void QPipeDevice::setAutoClose(bool autoClose)
{
	_autoClose = autoClose;
}

qint64 QPipeDevice::readData(char *data, qint64 maxlen)
{
	Q_UNUSED(data)
	Q_UNUSED(maxlen)
	return 0;
}

qint64 QPipeDevice::writeData(const char *data, qint64 len)
{
	auto nData = process(QByteArray(data, len));
	if(_sink) {
		auto r = _sink->write(nData);
		if(r != nData.size()) {
			setErrorString(tr("Failed to write transformed data to sink"));
			return -1;
		} else
			return len;
	} else
		return -1;
}

void QPipeDevice::init(bool buffered)
{
	Q_UNUSED(buffered)
}

QByteArray QPipeDevice::process(QByteArray &&data)
{
	return data;
}

void QPipeDevice::end() {}

bool QPipeDevice::open(OpenMode mode)
{
	if(mode.testFlag(ReadOnly) ||
	   mode.testFlag(Text))
		return false;
	else
		return open(mode.testFlag(Unbuffered));
}





QPointer<QPipeDevice> operator|(QPointer<QPipeDevice> source, QPointer<QPipeDevice> sink)
{
	return source->pipeTo(sink);
}

QPipeDevice &operator|(QPipeDevice &source, QPipeDevice &sink)
{
	source.pipeTo(&sink);
	return sink;
}

void operator|(QPipeDevice &source, QIODevice &sink)
{
	source.pipeTo(&sink);
}
