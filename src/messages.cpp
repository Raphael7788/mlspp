#include "messages.h"

namespace mls {

// UserInitKey

void
UserInitKey::sign(const SignaturePrivateKey& identity_priv)
{
  identity_key = identity_priv.public_key();
  auto tbs = to_be_signed();
  signature = identity_priv.sign(tbs);
}

bool
UserInitKey::verify() const
{
  auto tbs = to_be_signed();
  return identity_key.verify(tbs, signature);
}

bytes
UserInitKey::to_be_signed() const
{
  tls::ostream out;
  out << cipher_suites << init_keys << identity_key << algorithm;
  return out.bytes();
}

bool
operator==(const UserInitKey& lhs, const UserInitKey& rhs)
{
  return (lhs.cipher_suites == rhs.cipher_suites) &&
         (lhs.init_keys == rhs.init_keys) &&
         (lhs.identity_key == rhs.identity_key) &&
         (lhs.signature == rhs.signature);
}

tls::ostream&
operator<<(tls::ostream& out, const UserInitKey& obj)
{
  return out << obj.cipher_suites << obj.init_keys << obj.identity_key
             << obj.algorithm << obj.signature;
}

tls::istream&
operator>>(tls::istream& in, UserInitKey& obj)
{
  return in >> obj.cipher_suites >> obj.init_keys >> obj.identity_key >>
         obj.algorithm >> obj.signature;
}

// Welcome

bool
operator==(const Welcome& lhs, const Welcome& rhs)
{
  return (lhs.group_id == rhs.group_id) && (lhs.epoch == rhs.epoch) &&
         (lhs.roster == rhs.roster) && (lhs.tree == rhs.tree) &&
         (lhs.transcript == rhs.transcript) &&
         (lhs.init_secret == rhs.init_secret) &&
         (lhs.leaf_secret == rhs.leaf_secret);
}

tls::ostream&
operator<<(tls::ostream& out, const Welcome& obj)
{
  return out << obj.group_id << obj.epoch << obj.roster << obj.tree
             << obj.transcript << obj.init_secret << obj.leaf_secret;
}

tls::istream&
operator>>(tls::istream& in, Welcome& obj)
{
  return in >> obj.group_id >> obj.epoch >> obj.roster >> obj.tree >>
         obj.transcript >> obj.init_secret >> obj.leaf_secret;
}

// GroupOperationType

tls::ostream&
operator<<(tls::ostream& out, const GroupOperationType& obj)
{
  return out << uint8_t(obj);
}

tls::istream&
operator>>(tls::istream& in, GroupOperationType& obj)
{
  uint8_t type;
  in >> type;
  obj = GroupOperationType(type);
  return in;
}

// Add

const GroupOperationType Add::type = GroupOperationType::add;

bool
operator==(const Add& lhs, const Add& rhs)
{
  return (lhs.path == rhs.path) && (lhs.init_key == rhs.init_key);
}

tls::ostream&
operator<<(tls::ostream& out, const Add& obj)
{
  return out << obj.path << obj.init_key;
}

tls::istream&
operator>>(tls::istream& in, Add& obj)
{
  return in >> obj.path >> obj.init_key;
}

// Update

const GroupOperationType Update::type = GroupOperationType::update;

bool
operator==(const Update& lhs, const Update& rhs)
{
  return (lhs.path == rhs.path);
}

tls::ostream&
operator<<(tls::ostream& out, const Update& obj)
{
  return out << obj.path;
}

tls::istream&
operator>>(tls::istream& in, Update& obj)
{
  return in >> obj.path;
}

// Remove

const GroupOperationType Remove::type = GroupOperationType::remove;

bool
operator==(const Remove& lhs, const Remove& rhs)
{
  return (lhs.path == rhs.path);
}

tls::ostream&
operator<<(tls::ostream& out, const Remove& obj)
{
  return out << obj.removed << obj.path;
}

tls::istream&
operator>>(tls::istream& in, Remove& obj)
{
  return in >> obj.removed >> obj.path;
}

// GroupOperation
bool
operator==(const GroupOperation& lhs, const GroupOperation& rhs)
{
  return (lhs.type == rhs.type) &&
         (((lhs.type == GroupOperationType::add) && (lhs.add == rhs.add)) ||
          ((lhs.type == GroupOperationType::update) &&
           (lhs.update == rhs.update)) ||
          ((lhs.type == GroupOperationType::remove) &&
           (lhs.remove == rhs.remove)));
}

tls::ostream&
operator<<(tls::ostream& out, const GroupOperation& obj)
{
  out << obj.type;

  switch (obj.type) {
    case GroupOperationType::add:
      return out << obj.add;
    case GroupOperationType::update:
      return out << obj.update;
    case GroupOperationType::remove:
      return out << obj.remove;
  }

  throw InvalidParameterError("Unknown group operation type");
}

tls::istream&
operator>>(tls::istream& in, GroupOperation& obj)
{
  in >> obj.type;

  switch (obj.type) {
    case GroupOperationType::add:
      return in >> obj.add;
    case GroupOperationType::update:
      return in >> obj.update;
    case GroupOperationType::remove:
      return in >> obj.remove;
  }

  throw InvalidParameterError("Unknown group operation type");
}

// Handshake
bool
operator==(const Handshake& lhs, const Handshake& rhs)
{
  return (lhs.prior_epoch == rhs.prior_epoch) &&
         (lhs.operation == rhs.operation) &&
         (lhs.signer_index == rhs.signer_index) &&
         (lhs.signature == rhs.signature);
}

tls::ostream&
operator<<(tls::ostream& out, const Handshake& obj)
{
  return out << obj.prior_epoch << obj.operation << obj.signer_index
             << obj.signature;
}

tls::istream&
operator>>(tls::istream& in, Handshake& obj)
{
  return in >> obj.prior_epoch >> obj.operation >> obj.signer_index >>
         obj.signature;
}

} // namespace mls
