#include "setpour.h"

#include "apigateway.h"

SetPour::SetPour(ApiGateway *gateway)
    : ApiMessage (gateway, MessageType::MessageSetPour),
      m_farmId(1),
      m_positionId(1)
{

}

void SetPour::sendMessage()
{
    QNetworkRequest request(buildEndpointAjaxUrl("farm", {
        { "mode", "garden_water" },
        { "farm", QString::number(m_farmId) },
        { "position", QString::number(m_positionId) },
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

