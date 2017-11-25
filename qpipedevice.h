#ifndef QPIPEDEVICE_H
#define QPIPEDEVICE_H

#include <QIODevice>

class QPipeDevice : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QIODevice* source READ source WRITE setSource)
	Q_PROPERTY(QIODevice* sink READ sink WRITE setSink)

	Q_PROPERTY(qint64 blockSize READ blockSize WRITE setBlockSize RESET resetBlockSize)
	Q_PROPERTY(bool autoClose READ autoClose WRITE setAutoClose)

public:
	explicit QPipeDevice(QObject *parent = nullptr);

	QIODevice* source() const;
	QIODevice* sink() const;
	QPipeDevice* sourcePipe() const;
	QPipeDevice* sinkPipe() const;

	void pipeTo(QPipeDevice *pipeSink);
	void pipeFrom(QPipeDevice *pipeSource);

	QPipeDevice &operator|(QPipeDevice &other);
	void operator|(QIODevice *device);

	qint64 blockSize() const;
	bool autoClose() const;

public slots:
	void setSource(QIODevice* source);
	void setSink(QIODevice* sink);
	void setBlockSize(qint64 blockSize);
	void resetBlockSize();
	void setAutoClose(bool autoClose);

protected:
	virtual QByteArray process(QByteArray data);
	virtual void end();

private slots:
	void readyRead();
	void readClosed();
	void close();
	void pipeData(const QByteArray &data);

private:
	QIODevice* _source;
	QPipeDevice *_sourcePipe;
	QIODevice* _sink;
	QPipeDevice *_sinkPipe;

	qint64 _blockSize;
	bool _autoClose;
};

QPipeDevice &operator|(QIODevice *device, QPipeDevice &pipe);

#endif // QPIPEDEVICE_H
