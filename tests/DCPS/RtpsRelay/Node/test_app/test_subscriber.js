"use strict";

var DOMAIN_ID = 32;
var ddsCerts = process.env.DDS_ROOT + "/tests/security/certs/identity";

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
      ddsCerts + "/test_participant_02_cert.pem"},

    {name: "dds.sec.auth.private_key", value: "file:" +
      ddsCerts + "/test_participant_02_private_key.pem"},

    {name: "dds.sec.access.permissions", value: "file:" +
      "security/sub_permissions_signed.p7s"},

  ]};
}

function init_opendds(opendds_addon) {
  if (secure) {
    return opendds_addon.initialize('-DCPSConfigFile', 'rtps_disc.ini');
  } else {
    return opendds_addon.initialize();
  }
}

var opendds_addon = require('/opt/node-opendds/lib/node-opendds'),
  RelayTopicSubscriber = require('./RelayTopicSubscriber'),
  topic_lib = opendds_addon.load('idl/OpenDDS_RtpsRelay'),
  factory = init_opendds(opendds_addon),
  participant = factory.create_participant(DOMAIN_ID, domain_part_qos);

if (!topic_lib) {
  throw 'Failed to load shared library';
}

// Listen for the RtpsReplay topics
try {
  var reader = new RelayTopicSubscriber();
  reader.listen(participant, 'Writers', 'RtpsRelay::WriterEntry');
  reader.listen(participant, 'Readers', 'RtpsRelay::ReaderEntry');
  reader.listen(participant, 'Participant Entry', 'RtpsRelay::ParticipantEntry');
  reader.listen(participant, 'Responsible Relay', 'RtpsRelay::GuidNameAddress');
  reader.listen(participant, 'Participant Statistics', 'RtpsRelay::ParticipantStatistics');
  reader.listen(participant, 'Handler Statistics', 'RtpsRelay::HandlerStatistics');
  reader.listen(participant, 'Domain Statistics', 'RtpsRelay::DomainStatistics');
} catch (e) {
  console.log(e);
}

process.on('exit', function () {
  factory.delete_participant(participant); // optional
  opendds_addon.finalize(factory);
});
