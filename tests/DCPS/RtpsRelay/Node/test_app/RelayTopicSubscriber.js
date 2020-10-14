"use strict";

var dds_inf = 0x7fffffff,
  infinite = {sec: dds_inf, nanosec: dds_inf};

function RelayTopicSubscriber() {
  this.sample_count = 0;
}

RelayTopicSubscriber.prototype.run = function(participant, topic_name, topic_type) {

  console.log("subscribing for " + topic_name + " on " + topic_type);

  var reader = participant.subscribe(topic_name, topic_type, null,
    function (dr, sinfo, sample) {
      try {
        if (sinfo.valid_data == true) {
          console.log('Recv ' + topic_name);
          log('Data:', sample);
          // log('Sample Info:', sinfo);
        } else {
          log('Invalid Data', sinfo);
          participant.unsubscribe(reader);
        }
      } catch (e) {
        console.log("Error in callback: " + e);
      }
    });
}

RelayTopicSubscriber.prototype.listen = function(participant, topic_name, topic_type) {
  try {
    this.run(participant, topic_name, topic_type);
  } catch (e) {
    console.log(e);
  }
}

function log(label, object) {
  console.log(label + ': ' + JSON.stringify(object, null, 2));
}

module.exports = RelayTopicSubscriber;

