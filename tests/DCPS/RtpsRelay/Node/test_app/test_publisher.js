"use strict";

var DOMAIN_ID = 32;
var ddsCerts = process.env.DDS_ROOT + "/tests/security/certs/identity";

var max_cycles = 10;

var domain_part_qos = {
  user_data: 'foo',
  entity_factory: {
    autoenable_created_entities: true
  }
};

var secure = process.argv.includes('--secure');
if (secure) {
  qos.property = { value: [

    {name: "dds.sec.auth.identity_ca", value: "file:" +
      ddsCerts + "/identity_ca_cert.pem"},

    {name: "dds.sec.access.permissions_ca", value: "file:" +
      ddsCerts + "/identity_ca_cert.pem"},

    {name: "dds.sec.access.governance", value: "file:" +
      "security/governance_signed.p7s"},

    {name: "dds.sec.auth.identity_certificate", value: "file:" +
      ddsCerts + "/test_participant_01_cert.pem"},

    {name: "dds.sec.auth.private_key", value: "file:" +
      ddsCerts + "/test_participant_01_private_key.pem"},

    {name: "dds.sec.access.permissions", value: "file:" +
      "security/pub_permissions_signed.p7s"},

  ]};
}

// Constant QoS and GUID values for testing

// initial_TopicQos_.topic_data = initial_TopicDataQosPolicy_;
// initial_TopicQos_.durability = initial_DurabilityQosPolicy_;
// initial_TopicQos_.durability_service = initial_DurabilityServiceQosPolicy_;
// initial_TopicQos_.deadline = initial_DeadlineQosPolicy_;
// initial_TopicQos_.latency_budget = initial_LatencyBudgetQosPolicy_;
// initial_TopicQos_.liveliness = initial_LivelinessQosPolicy_;
// initial_TopicQos_.reliability = initial_ReliabilityQosPolicy_;
// initial_TopicQos_.destination_order = initial_DestinationOrderQosPolicy_;
// initial_TopicQos_.history = initial_HistoryQosPolicy_;
// initial_TopicQos_.resource_limits = initial_ResourceLimitsQosPolicy_;
// initial_TopicQos_.transport_priority = initial_TransportPriorityQosPolicy_;
// initial_TopicQos_.lifespan = initial_LifespanQosPolicy_;
// initial_TopicQos_.ownership = initial_OwnershipQosPolicy_;


initial_DeadlineQosPolicy_.period.sec = DDS::DURATION_INFINITE_SEC;
initial_DeadlineQosPolicy_.period.nanosec = DDS::DURATION_INFINITE_NSEC;

initial_LatencyBudgetQosPolicy_.duration.sec = DDS::DURATION_ZERO_SEC;
initial_LatencyBudgetQosPolicy_.duration.nanosec = DDS::DURATION_ZERO_NSEC;

initial_LivelinessQosPolicy_.kind = DDS::AUTOMATIC_LIVELINESS_QOS;
initial_LivelinessQosPolicy_.lease_duration.sec = DDS::DURATION_INFINITE_SEC;
initial_LivelinessQosPolicy_.lease_duration.nanosec = DDS::DURATION_INFINITE_NSEC;

initial_ReliabilityQosPolicy_.kind = DDS::BEST_EFFORT_RELIABILITY_QOS;
initial_ReliabilityQosPolicy_.max_blocking_time.sec = DDS::DURATION_INFINITE_SEC;
initial_ReliabilityQosPolicy_.max_blocking_time.nanosec = DDS::DURATION_INFINITE_NSEC;

initial_DestinationOrderQosPolicy_.kind = DDS::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;


initial_HistoryQosPolicy_.kind = DDS::KEEP_LAST_HISTORY_QOS;
initial_HistoryQosPolicy_.depth = 1;

initial_ResourceLimitsQosPolicy_.max_samples = DDS::LENGTH_UNLIMITED;
initial_ResourceLimitsQosPolicy_.max_instances = DDS::LENGTH_UNLIMITED;
initial_ResourceLimitsQosPolicy_.max_samples_per_instance = DDS::LENGTH_UNLIMITED;

initial_TransportPriorityQosPolicy_.value = 0;
initial_OwnershipQosPolicy_.kind = DDS::SHARED_OWNERSHIP_QOS;
initial_LifespanQosPolicy_.duration.sec = DDS::DURATION_INFINITE_SEC;
initial_LifespanQosPolicy_.duration.nanosec = DDS::DURATION_INFINITE_NSEC;

//
const relay_qos = null;
const part_guid = {
  guidPrefix: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11],
  entityId: {
    entityKey: [0, 1, 2],
    entityKind: 0
  }
};
const writer_qos = null;
// {
//   latency_budget: {
//     sec: 1,
//     nanosec: 0
//   },
//   liveliness: {
//     lease_duration: {
//       sec: 5,
//       nanosec: 0
//     }
//   }
// };

function init_opendds(opendds_addon) {
  if (secure) {
    return opendds_addon.initialize('-DCPSConfigFile', 'rtps_disc.ini');
  } else {
    return opendds_addon.initialize();
  }
}

// Setup default variables and pull in requirements
var opendds_addon = require('/opt/node-opendds/lib/node-opendds'),
  publishers = require('./RelayTopicPublisher'),
  topic_lib = opendds_addon.load('idl/OpenDDS_RtpsRelay'),
  util = require('util'),
  factory = init_opendds(opendds_addon),
  participant = factory.create_participant(DOMAIN_ID, domain_part_qos),
  loop_counter = 0,
  interval_timer = null;


function publish_topics(writers) {
  // Stop the timer after <max_cycles> intervals
  if (++loop_counter > max_cycles) {
    console.log('Publisher exiting');
    clearInterval(interval_timer);
  } else {
    writers.forEach(writer => {
      writer.publish();
    });
  }
}


try {
  if (!topic_lib) {
    throw 'Failed to load shared library';
  }

  var writers = [];
  writers.push(new publishers.ParticpantStatsPublisher(participant, writer_qos));
  writers.push(new publishers.HandlerStatsPublisher(participant, writer_qos));
  writers.push(new publishers.DomainStatsPublisher(participant, writer_qos));
  writers.push(new publishers.ReaderEntryTopicPublisher(participant, writer_qos));
  writers.push(new publishers.WriterEntryTopicPublisher(participant, writer_qos));
  writers.push(new publishers.ParticipantEntryTopicPublisher(participant, writer_qos));
  writers.push(new publishers.GuidNameAddressTopicPublisher(participant, writer_qos));

  interval_timer = setInterval(function () { publish_topics(writers); }, 3000);

} catch (e) {
  console.log(e);
}

process.on('exit', function () {
  factory.delete_participant(participant); // optional
  opendds_addon.finalize(factory);
});
