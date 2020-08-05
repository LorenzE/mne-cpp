#include "dataloadermodel.h"
#include "../data/filepathdata.h"

#include <QtCore/QEvent>
#include <QtCore/QDir>

#include <QtWidgets/QFileDialog>

DataLoaderModel::DataLoaderModel()
: m_pLoadDataButton(new QPushButton("Load file"))
{
    connect(m_pLoadDataButton, &QPushButton::pressed,
            this, &DataLoaderModel::onLoadButtonClicked);
}


unsigned int DataLoaderModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
    {
    case PortType::In:
        result = 0;
        break;

    case PortType::Out:
        result = 1;

    default:
        break;
    }

    return result;
}


NodeDataType DataLoaderModel::dataType(PortType, PortIndex) const
{
    return FilePathData().type();
}


std::shared_ptr<NodeData> DataLoaderModel::outData(PortIndex)
{
    return std::make_shared<FilePathData>(m_sDataPath);
}


void DataLoaderModel::onLoadButtonClicked()
{
    //Get the path
    QString sFilePath = QFileDialog::getOpenFileName(Q_NULLPTR,
                                                     tr("Open File"),
                                                     QDir::currentPath()+"/MNE-sample-data",
                                                     tr("Fiff file(*.fif *.fiff)"));

    if(QFileInfo(sFilePath).exists()) {
        m_sDataPath = sFilePath;
        emit dataUpdated(0);
    }
}
