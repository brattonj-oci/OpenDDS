"use strict";

const part_guid = {
  guidPrefix: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11],
  entityId: {
    entityKey: [0, 1, 2],
    entityKind: 0
  }
};

function hrtime_to_duration(value) {
  return {sec: value[0], nanosec: value[1]};
}

class RelayTopicPublisher {
  constructor(participant, qos, topic_name, topic_type) {
    this.writer = participant.create_datawriter(topic_name, topic_type, qos);
    this.topic_name = topic_name;

    // Setup a last published timestamp
    this.last_publish = process.hrtime();
  }

  get_interval() {
    // Simplest way to do this, but not optimal
    var value = process.hrtime(this.last_publish);
    return {sec: value[0], nanosec: value[1]};
  }

  publish() {
    var sample = this.create_sample();
    var handle = this.writer.register_instance(sample);
    var retcode = this.writer.write(sample, handle);

    this.last_publish = process.hrtime();
  }

  stop() {
  }

  create_sample() {
    return {};
  }
}

class ParticpantStatsPublisher extends RelayTopicPublisher {
  constructor(participant, qos) {
    super(participant, qos, 'Participant Statistics', 'RtpsRelay::ParticipantStatistics');
  }

  create_sample() {
    return {
      guid: part_guid,
      name: "MyTestParticipantStats",
      interval: this.get_interval(),
      messages_in: 42,
      bytes_in: 2048,
      messages_out: 2,
      bytes_out: 1024,
      max_fan_out: 3
    };
  }
}

class HandlerStatsPublisher extends RelayTopicPublisher {
  constructor(participant, qos) {
    super(participant, qos, 'Handler Statistics', 'RtpsRelay::HandlerStatistics');
  }

  create_sample() {
    return {
      application_participant_guid: part_guid,
      name: "MyTestHandlerStats",
      interval: this.get_interval(),
      messages_in: 42,
      bytes_in: 2048,
      messages_out: 2,
      bytes_out: 1024,
      max_fan_out: 3,
      max_queue_size: 5,
      max_queue_latency: {sec: 0, nanosec: 2230000000},
      local_active_participants: 1,
      error_count: 2,
      governor_count: 4
    }
  }
}

class DomainStatsPublisher extends RelayTopicPublisher {
  constructor(participant, qos) {
    super(participant, qos, 'Domain Statistics', 'RtpsRelay::DomainStatistics');
  }

  create_sample() {
    return {
      application_participant_guid: part_guid,
      interval: this.get_interval(),
      local_participants: 1,
      local_writers: 2,
      local_readers: 3,
      total_writers: 4,
      total_readers: 5
    };
  }
}

class WriterEntryTopicPublisher extends RelayTopicPublisher {
  constructor(participant, qos) {
    super(participant, qos, "Writers", 'RtpsRelay::WriterEntry');
  }

  create_sample() {
    return {
      guid: part_guid,
      topic_name: "Namespace::Name",
      type_name: "MyTopic"
    };
  }
}

class ReaderEntryTopicPublisher extends RelayTopicPublisher {
  constructor(participant, qos) {
    super(participant, qos, "Readers", 'RtpsRelay::ReaderEntry');
  }

  create_sample() {
    return {
      guid: part_guid,
      topic_name: "Namespace::Name",
      type_name: "MyTopic"
    };
  }
}

class ParticipantEntryTopicPublisher extends RelayTopicPublisher {
  constructor(participant, qos) {
    super(participant, qos, "Participant Entry", 'RtpsRelay::ParticipantEntry');
  }

  create_sample() {
    return {
      guid: part_guid,
      user_data: [0, 1, 2]
    };
  }
}

class GuidNameAddressTopicPublisher extends RelayTopicPublisher {
  constructor(participant, qos) {
    super(participant, qos, "Responsible Relay", 'RtpsRelay::GuidNameAddress');
  }

  create_sample() {
    return {
      guid: part_guid,
      name: "GuidName",
      address: "10.8.0.5"
    };
  }
}

module.exports = {
  RelayTopicPublisher: RelayTopicPublisher,
  ParticpantStatsPublisher: ParticpantStatsPublisher,
  HandlerStatsPublisher: HandlerStatsPublisher,
  DomainStatsPublisher: DomainStatsPublisher,
  WriterEntryTopicPublisher: WriterEntryTopicPublisher,
  ReaderEntryTopicPublisher: ReaderEntryTopicPublisher,
  ParticipantEntryTopicPublisher: ParticipantEntryTopicPublisher,
  GuidNameAddressTopicPublisher: GuidNameAddressTopicPublisher
};