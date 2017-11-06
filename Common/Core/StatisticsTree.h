
#pragma once

#include "Statistics.h"
#include "StatisticsTree.h"
#include <map>
#include <memory>
#include <set>
#include <string>

namespace Realisim
{
namespace Core
{

    //-------------------------------------------------------------------------
    // This class holds a tree of statistics (see class Statistics). This is 
    // merely a convenient class to hold a hierarchy of statistics. The typical
    // use case is to profile many tasks/subtasks.
    //
    // Usage:
    //      StatisticsTree st;
    //      st.add("task1", someTimer);
    //      st.add("task1/subtask1", someTimer);
    //      st.add("task1/subtask2", someTimer);
    //
    //      ...
    //
    //      //display profiling info
    //      cout << "task1: " << st.getAverage("task1") << 
    //          "subtask1:" << st.getAverage("task1/subtask1") ...;
    //
    //      //display all profiling info
    //      const set<string>& keys = st.getKeys();
    //      for(auto key : keys)
    //      {
    //          cout << key << ": " << st.getAverage(key) << endl;
    //      }
    //
    // a value is enterer by using a key and a value via method add().
    // a key is in the form of a folder path: some/path/to/somewhere
    //
    // to retrieve the value of a key use methods getAverage(), getMaximum(),
    // getMinimum() and getStandardDeviation().
    //
    // It is posible to introspect if a hey exists with hasKey() and to
    // introspect if a key has childs with hasChilds().
    //
    class StatisticsTree
    {
    public:
        StatisticsTree();
        StatisticsTree(const StatisticsTree&) = delete;
        StatisticsTree& operator=(const StatisticsTree&) = delete;
        ~StatisticsTree();

        void add(const std::string& iKey, double iValue);
        void clear();
        double getAverage(const std::string& iKey) const;
        const std::set<std::string>& getKeys() const;
        double getMaximum(const std::string& iKey) const;
        double getMinimum(const std::string& iKey) const;
        int getNumberOfKeys() const;
        // std::vector<double> getSamples(const std::string& iKey) const;
        double getStandardDeviation(const std::string& iKey) const;
        bool hasChilds(const std::string& iKey) const;
        bool hasKey(const std::string& iKey) const;
        bool isKeepingSamples() const;
        void keepSamples(bool iK);
        // std::string toCsv() const;

    protected:
        struct Node
        {
            Node();
            Node(const Node&) = delete;
            Node& operator=(const Node&) = delete;
            ~Node() = default;
            
            std::string mKey;
            Statistics mStats;
            std::map<std::string, std::unique_ptr<Node>> mChilds;
            //std::map<std::string, Node*> mChilds;
        };

        Node* addKey(const std::string& iKey);
        const Node* findKey(const std::string& iKey) const;
        Node* findKey(const std::string& iKey);
        void getChildSummedAverage(const Node *ipNode, double *oValue);

        //--- data
        bool mKeepSamples;
        Node *mpRoot; //never null
        std::set<std::string> mKeys;
    };

}
} 

