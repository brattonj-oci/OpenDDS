"use strict";

var dds_inf = 0x7fffffff,
  infinite = {sec: dds_inf, nanosec: dds_inf};

function RelayTopicSubscriber() {
  this.sample_count = 0;
}

RelayTopicSubscriber.prototype.run = function(participant, topic_name, topic_type) {

  console.log("subscribing for " + topic_name + " on " + topic_type);

  var reader = participant.subscribe(topic_name, topic_type, null,
    // {
    // SubscriberQos: {
    //   presentation: {
    //     access_scope: 'INSTANCE_PRESENTATION_QOS',
    //     coherent_access: false,
    //     ordered_access: false
    //   },
    //   partition: ['*'],
    //   group_data: 'test'
    // },
    // DataReaderQos: {
    //   durability: 'VOLATILE_DURABILITY_QOS',
    //   latency_budget: {sec: 1, nanosec: 0},
    //   liveliness: {
    //     kind: 'AUTOMATIC_LIVELINESS_QOS',
    //     lease_duration: infinite
    //   },
    //   reliability: {
    //     kind: 'RELIABLE_RELIABILITY_QOS',
    //     max_blocking_time: {sec: 1, nanosec: 0}
    //   },
    //   destination_order: 'BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS',
    //   history: {
    //     kind: 'KEEP_LAST_HISTORY_QOS',
    //     depth: 10
    //   },
    //   resource_limits: {
    //     max_samples: 1000,
    //     max_instances: 100,
    //     max_samples_per_instance: 10
    //   },
    //   user_data: 'arbitrary string',
    //   ownership: 'SHARED_OWNERSHIP_QOS',
    //   time_based_filter: {sec: 0, nanosec: 0},
    //   reader_data_lifecycle: {
    //     autopurge_nowriter_samples_delay: infinite,
    //     autopurge_disposed_samples_delay: infinite
    //   }
    // }
  //},
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

