#ifndef QPIPEDEVICE_H
#define QPIPEDEVICE_H

#include <QIODevice>
#include <QPointer>

#include <functional>

class QPipeDevice : public QIODevice
{
	Q_OBJECT

	Q_PROPERTY(bool autoOpen READ autoOpen WRITE setAutoOpen)
	Q_PROPERTY(bool autoClose READ autoClose WRITE setAutoClose)

public:
	explicit QPipeDevice(QObject *parent = nullptr);

	QIODevice* sink() const;
	bool autoOpen() const;
	bool autoClose() const;

	QPipeDevice* pipeTo(QPipeDevice *sink);
	void pipeTo(QIODevice* sink);

	bool isSequential() const final;
	bool open(bool buffered = true);
	void close() final;

public slots:
	void setAutoOpen(bool autoOpen);
	void setAutoClose(bool autoClose);

protected:
	qint64 readData(char *data, qint64 maxlen) final;
	qint64 writeData(const char *data, qint64 len) final;

	virtual void init(bool buffered);
	virtual QByteArray process(QByteArray &&data);
	virtual void end();

private:
	QIODevice* _sink;

	qint64 _blockSize;
	bool _autoOpen;
	bool _autoClose;

	bool open(OpenMode mode) final;
};

template <typename T>
void operator|(QPointer<QPipeDevice> source, QPointer<T> sink) {
	static_assert(std::is_base_of<QIODevice, T>::value, "only QIODevice based classes can be piped");
	return source->pipeTo(sink);
}
QPointer<QPipeDevice> operator|(QPointer<QPipeDevice> source, QPointer<QPipeDevice> sink);

QPipeDevice &operator|(QPipeDevice &source, QPipeDevice &sink);
void operator|(QPipeDevice &source, QIODevice &sink);

#endif // QPIPEDEVICE_H
