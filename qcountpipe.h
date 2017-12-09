#ifndef QCOUNTPIPE_H
#define QCOUNTPIPE_H

#include "qpipedevice.h"

class QCountPipe : public QPipeDevice
{
	Q_OBJECT

	Q_PROPERTY(qint64 count READ count RESET resetCount)

public:
	explicit QCountPipe(QObject *parent = nullptr);

	qint64 count() const;

public slots:
	void resetCount();

protected:
	QByteArray process(QByteArray &&data) override;

private:
	qint64 _count;
};

#endif // QCOUNTPIPE_H
