#include "setplant.h"

#include "apigateway.h"

SetPlant::SetPlant(ApiGateway *gateway)
    : ApiMessage (gateway, MessageType::MessageSetPlant),
      m_farmId(1),
      m_positionId(1),
      m_plantId(1)
{

}

void SetPlant::sendMessage()
{
    QNetworkRequest request(buildEndpointAjaxUrl("farm", {
        { "mode", "garden_plant" },
        { "farm", QString::number(m_farmId) },
        { "position", QString::number(m_positionId) },
        { "pflanze[]", QString::number(m_plantId) },
        { "feld[]", "1" },
        { "felder[]", "1,2"}
    }));

    buildHeaders(request);

    auto reply = m_manager->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply] {
        const auto contents = reply->readAll();
        qDebug() << contents;
    });

}

