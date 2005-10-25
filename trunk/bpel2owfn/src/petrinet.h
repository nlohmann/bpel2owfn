#ifndef PETRINET_H
#define PETRINET_H



#include <string>
#include <iostream>
#include <vector>
#include <set>
#include "bpel-kc-k.h"       // generated by kimwitu++
#include "bpel-kc-rk.h"      // generated by kimwitu++
#include "bpel-kc-unpk.h"    // generated by kimwitu++



class Node
{
  public:
    std::string name;
    std::set<std::string> roles;
};


class Arc
{
  public:
    Node *source;
    Node *target;    
    Arc(Node *source, Node *target);
};


class Transition: public Node
{
  private:
    unsigned int type;

  public:
    Transition(std::string name, std::string role, unsigned int type);
};



class Place: public Node
{
  private:
    unsigned int type;
    
  public:
    Place(std::string name, std::string role, unsigned int type);
};



class PetriNet
{
  public:
    Place *newPlace(std::string name, std::string role, unsigned int type);
    Transition *newTransition(std::string name, std::string role, unsigned int type);
    Arc *newArc(Node *source, Node *target);
    void information();
    void drawDot();
    void mergePlaces(Place *p1, Place *p2);
    void mergePlaces(std::string name1, std::string name2);
    void mergePlaces(kc::impl_activity* act1, std::string role1, kc::impl_activity* act2, std::string role2);
    void mergeTransitions(Transition *t1, Transition *t2);
    void removePlace(Place *p);
    void removeTransition(Transition *t);
    void removeArc(Arc *f);
    Place *findPlace(std::string name);
    Place *findPlaceRole(std::string role);
    std::set<Node *> preset(Node *n);
    std::set<Node *> postset(Node *n);

    void simplify();

    PetriNet();


  private:
    unsigned int places;
    unsigned int transitions;
    unsigned int arcs;
    std::vector<Place *> P;
    std::vector<Transition *> T;
    std::vector<Arc *> F;
};

#endif
