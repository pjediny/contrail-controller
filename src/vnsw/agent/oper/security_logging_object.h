/*
 * Copyright (c) 2017 Juniper Networks, Inc. All rights reserved.
 */

#ifndef __AGENT_OPER_SECURITY_LOGGING_OBJECT_H
#define __AGENT_OPER_SECURITY_LOGGING_OBJECT_H

#include <cmn/agent.h>
#include <oper_db.h>
#include <cmn/index_vector.h>

class Agent;
class DB;

struct SecurityLoggingObjectKey : public AgentOperDBKey {
public:
    SecurityLoggingObjectKey(const boost::uuids::uuid &uuid):
        uuid_(uuid) {}

    SecurityLoggingObjectKey(const SecurityLoggingObjectKey &rhs):
        uuid_(rhs.uuid_) {}

    bool IsLess(const SecurityLoggingObjectKey &rhs) const {
        return uuid_ < rhs.uuid_;
    }

    boost::uuids::uuid uuid_;
};

struct SecurityLoggingObjectData : public AgentOperDBData {
public:
    SecurityLoggingObjectData(const Agent *agent, IFMapNode *node,
              const std::vector<autogen::SecurityLoggingObjectRuleEntryType> &lst,
              const int &rate, const std::string &name):
    AgentOperDBData(agent, node), rules_(lst), rate_(rate), name_(name),
        firewall_policy_list_(), firewall_rule_list_() {}

    std::vector<autogen::SecurityLoggingObjectRuleEntryType> rules_;
    int rate_;
    std::string name_;
    UuidList firewall_policy_list_;
    UuidList firewall_rule_list_;
};

class SecurityLoggingObject:
    AgentRefCount<SecurityLoggingObject>, public AgentOperDBEntry {
public:
    SecurityLoggingObject(const boost::uuids::uuid &uuid);
    virtual ~SecurityLoggingObject();

    KeyPtr GetDBRequestKey() const;
    virtual void SetKey(const DBRequestKey *key);
    std::string ToString() const;
    virtual bool IsLess(const DBEntry &rhs) const;
    virtual bool DBEntrySandesh(Sandesh *resp, std::string &name) const;
    virtual bool Change(const DBRequest *req);
    bool IsEqual(const std::vector<autogen::SecurityLoggingObjectRuleEntryType>
                 &lhs,
                 const std::vector<autogen::SecurityLoggingObjectRuleEntryType>
                 &rhs) const;

    uint32_t GetRefCount() const {
        return AgentRefCount<SecurityLoggingObject>::GetRefCount();
    }

    const boost::uuids::uuid& uuid() const {
        return uuid_;
    }
    const std::vector<autogen::SecurityLoggingObjectRuleEntryType> & rules()
        const {
        return rules_;
    }
    int rate() const { return rate_; }
    const std::string& name() const {
        return name_;
    }

private:
    boost::uuids::uuid uuid_;
    std::vector<autogen::SecurityLoggingObjectRuleEntryType> rules_;
    int rate_;
    std::string name_;
    UuidList firewall_policy_list_;
    UuidList firewall_rule_list_;
    DISALLOW_COPY_AND_ASSIGN(SecurityLoggingObject);
};

class SecurityLoggingObjectTable : public AgentOperDBTable {
public:
    SecurityLoggingObjectTable(Agent *agent, DB *db, const std::string &name);
    virtual ~SecurityLoggingObjectTable();

    static DBTableBase *CreateTable(Agent *agent, DB *db,
                                    const std::string &name);
    virtual std::auto_ptr<DBEntry> AllocEntry(const DBRequestKey *k) const;
    virtual DBEntry *OperDBAdd(const DBRequest *req);
    virtual bool OperDBOnChange(DBEntry *entry, const DBRequest *req);
    virtual bool OperDBDelete(DBEntry *entry, const DBRequest *req);
    virtual bool IFNodeToReq(IFMapNode *node, DBRequest &req,
            const boost::uuids::uuid &u);
    virtual bool IFNodeToUuid(IFMapNode *node, boost::uuids::uuid &u);
    bool ProcessConfig(IFMapNode *node, DBRequest &req,
                       const boost::uuids::uuid &u);
    virtual AgentSandeshPtr GetAgentSandesh(const AgentSandeshArguments *args,
                                            const std::string &context);

private:
    SecurityLoggingObjectData* BuildData(IFMapNode *node) const;
    DISALLOW_COPY_AND_ASSIGN(SecurityLoggingObjectTable);
};
#endif
