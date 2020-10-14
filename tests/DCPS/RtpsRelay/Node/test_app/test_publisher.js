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
const part_guid = {
  guidPrefix: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11],
  entityId: {
    entityKey: [0, 1, 2],
    entityKind: 0
  }
};
const writer_qos = null; // Default


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
  topic_lib = opendds_addon.load('idl/OpenDDS_RtpsRelayNode'),
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
