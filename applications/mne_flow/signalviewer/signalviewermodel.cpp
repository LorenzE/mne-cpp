#include "signalviewermodel.h"

#include <QtCore/QEvent>
#include <QtCore/QDir>

#include <QtWidgets/QFileDialog>

#include <nodes/DataModelRegistry>

#include "../data/filepathdata.h"
#include "../data/scalingdata.h"
#include "fiffrawviewmodel.h"
#include "fiffrawviewdelegate.h"

SignalViewerModel::SignalViewerModel()
: m_pFiffRawView(new FiffRawView())
{
    m_pFiffRawView->setMinimumSize(256, 256);
    m_pFiffRawView->setAttribute(Qt::WA_DeleteOnClose, false);
    m_pFiffRawView->setDelegate(QSharedPointer<FiffRawViewDelegate>::create());
}


unsigned int SignalViewerModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
    {
    case PortType::In:
        result = 2;
        break;

    case PortType::Out:
        result = 0;

    default:
        break;
    }

    return result;
}


NodeDataType SignalViewerModel::dataType(PortType portType, PortIndex portIndex) const
{
    if(portType == PortType::In) {
        switch(portIndex)
        {
            case 0:
                return FilePathData().type();

            case 1:
                return ScalingData().type();
        }
    }
}


std::shared_ptr<NodeData> SignalViewerModel::outData(PortIndex portIndex)
{
    return nullptr;
}


void SignalViewerModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex portIndex)
{
    switch (portIndex)
    {
    case 0:
        if(auto d = std::dynamic_pointer_cast<FilePathData>(nodeData))
        {
            QString sFilePath = d->filePath();

            if(sFilePath.contains(".fif") || sFilePath.contains(".fiff")) {
                FiffRawViewModel::SPtr pModel = FiffRawViewModel::SPtr::create(sFilePath);

                if(pModel->isInit()) {
                    m_pFiffRawView->setModel(pModel);
                }
            }
        }
        break;

    case 1:
        if(auto d = std::dynamic_pointer_cast<ScalingData>(nodeData))
        {
            QMap<qint32, float> scalingMap = d->scalingMap();
            m_pFiffRawView->setScalingMap(scalingMap);
        }
        break;

    default:
        break;
    }
}
