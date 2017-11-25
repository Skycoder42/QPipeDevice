#ifndef QPIPEDEVICE_H
#define QPIPEDEVICE_H

#include <QIODevice>
#include <QPointer>

class QPipeDevice : public QObject
{
	Q_OBJECT

	Q_PROPERTY(qint64 blockSize READ blockSize WRITE setBlockSize RESET resetBlockSize)
	Q_PROPERTY(bool autoClose READ autoClose WRITE setAutoClose)

public:
	explicit QPipeDevice(QObject *parent = nullptr);

	QIODevice* sourceDevice() const;
	QIODevice* sinkDevice() const;
	QPipeDevice* sourcePipe() const;
	QPipeDevice* sinkPipe() const;

	qint64 blockSize() const;
	bool autoClose() const;

	QPipeDevice* pipeTo(QPipeDevice *sink);
	void pipeTo(QIODevice* sink);
	QPipeDevice* pipeFrom(QPipeDevice *source);
	QPipeDevice* pipeFrom(QIODevice* source);

public slots:
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

QPointer<QPipeDevice> operator|(QPointer<QPipeDevice> source, QPointer<QPipeDevice> sink);
void operator|(QPointer<QPipeDevice> source, QPointer<QIODevice> sink);
QPointer<QPipeDevice> operator|(QPointer<QIODevice> source, QPointer<QPipeDevice> sink);

#endif // QPIPEDEVICE_H
