#pragma once

#include <iostream>

#include <QtCore/QObject>
#include <QtWidgets/QPushButton>

#include <nodes/NodeDataModel>

#include <disp/viewers/scalingview.h>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class ScalingModel : public NodeDataModel
{
    Q_OBJECT

public:
    ScalingModel();

    virtual ~ScalingModel() {}

public:

    QString caption() const override
    { return QString("Scaling"); }

    QString name() const override
    { return QString("Scaling"); }

public:

    virtual QString modelName() const
    { return QString("Scaling"); }

    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData>, int) override
    { }

    QWidget * embeddedWidget() override
    { return m_pScalingView; }

    bool resizable() const override
    { return false; }

private:

    void onScalingChanged(const QMap<qint32, float>& scalingMap);

private:

    DISPLIB::ScalingView * m_pScalingView;
    QMap<qint32, float> m_scalingMap;
};
