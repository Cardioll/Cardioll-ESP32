#include "cardiollTClient.h"

/**
 * createDocument - create document from ecg readings 
 */
void CardiollClient::createDocument(Cardioll &ecg, Document<Values::Value> &doc, Document<Values::Value> &alert)
{
    Cardioll::ECGFeatures features = ecg.GetFeatures();

    Values::DoubleValue rrInterval(features.rr_interval);
    Values::DoubleValue prInterval(features.pr_interval);
    Values::DoubleValue hr(features.heart_rate);
    Values::DoubleValue qtInterval(features.qt_interval);
    Values::DoubleValue qtcInterval(features.qtc_interval);
    Values::DoubleValue qrsDuration(features.qrs_duration);
    Values::DoubleValue averageRR(features.average_rr_interval);

   
    doc.add("rrInterval", Values::Value(rrInterval)); 
    doc.add("prInterval", Values::Value(prInterval));
    doc.add("heartRate", Values::Value(hr));
    doc.add("qtInterval", Values::Value(qtInterval));
    doc.add("QTc", Values::Value(qtcInterval));
    doc.add("qrsDuration", Values::Value(qrsDuration));
    doc.add("averageRR", Values::Value(averageRR));

    alert.add("type", Values::Value(Values::StringValue("Trachycardia"))); 

    String has_trachy = detect_trachidia(patent_info.age, patent_info.sex,
     (features.heart_rate / 1000), (features.pr_interval / 1000), (features.qrs_duration / 1000),
      (features.qt_interval / 1000), (features.qtc_interval / 1000), (features.average_rr_interval / 1000));
    

    alert.add("message", Values::Value(Values::StringValue(has_trachy)));
}