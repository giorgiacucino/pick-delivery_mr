// Auto-generated. Do not edit!

// (in-package pick_delivery.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class c_to_s {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.resp = null;
      this.idrob = null;
    }
    else {
      if (initObj.hasOwnProperty('resp')) {
        this.resp = initObj.resp
      }
      else {
        this.resp = 0;
      }
      if (initObj.hasOwnProperty('idrob')) {
        this.idrob = initObj.idrob
      }
      else {
        this.idrob = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type c_to_s
    // Serialize message field [resp]
    bufferOffset = _serializer.int64(obj.resp, buffer, bufferOffset);
    // Serialize message field [idrob]
    bufferOffset = _serializer.int64(obj.idrob, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type c_to_s
    let len;
    let data = new c_to_s(null);
    // Deserialize message field [resp]
    data.resp = _deserializer.int64(buffer, bufferOffset);
    // Deserialize message field [idrob]
    data.idrob = _deserializer.int64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 16;
  }

  static datatype() {
    // Returns string type for a message object
    return 'pick_delivery/c_to_s';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'e9e9ddd2ccd7a9c901bb09b4fb8d85be';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int64 resp
    int64 idrob
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new c_to_s(null);
    if (msg.resp !== undefined) {
      resolved.resp = msg.resp;
    }
    else {
      resolved.resp = 0
    }

    if (msg.idrob !== undefined) {
      resolved.idrob = msg.idrob;
    }
    else {
      resolved.idrob = 0
    }

    return resolved;
    }
};

module.exports = c_to_s;
