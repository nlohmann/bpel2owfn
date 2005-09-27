%token K_ASSIGN K_CASE K_CATCH K_CATCHALL K_COMPENSATE K_COMPENSATIONHANDLER
%token K_COPY K_CORRELATION K_CORRELATIONS K_CORRELATIONSET K_CORRELATIONSETS
%token K_EMPTY K_EVENTHANDLERS K_FAULTHANDLERS K_FLOW K_FROM K_INVOKE K_LINK
%token K_LINKS K_ONALARM K_ONMESSAGE K_OTHERWISE K_PARTNER K_PARTNERLINK
%token K_PARTNERLINKS K_PARTNERS K_PICK K_PROCESS K_RECEIVE K_REPLY K_SCOPE
%token K_SEQUENCE K_SOURCE K_SWITCH K_TARGET K_TERMINATE K_THROW K_TO
%token K_VARIABLE K_VARIABLES K_WAIT K_WHILE

%token X_OPEN X_SLASH X_CLOSE X_NEXT X_NAME X_STRING X_EQUALS X_ELEMENTNAME

%start tProcess


%token_table


%{

#define YYDEBUG 1
#define YYERROR_VERBOSE 1  ///< for verbose error messages

extern int yyerror(const char *);
extern char* yytext;

extern int yylex();

%}


%%

/*---------------------------------------------------------------------------*/

/*
  <process name="ncname" targetNamespace="uri" 
           queryLanguage="anyURI"?
           expressionLanguage="anyURI"?
           suppressJoinFailure="yes|no"?
           enableInstanceCompensation="yes|no"?
           abstractProcess="yes|no"?
           xmlns="http://schemas.xmlsoap.org/ws/2003/03/business-process/">

    <partnerLinks>?
    <partners>?
    <variables>?
    <correlationSets>?
    <faultHandlers>?
    <compensationHandler>?
    <eventHandlers>?
    
    activity
  </process>

  The top-level attributes are as follows:

   * queryLanguage. This attribute specifies the XML query language used for
     selection of nodes in assignment, property definition, and other uses.
     The default for this attribute is XPath 1.0, represented by the URI of the
     XPath 1.0 specification: http://www.w3.org/TR/1999/REC-xpath-19991116.

   * expressionLanguage. This attribute specifies the expression language used
     in the process. The default for this attribute is XPath 1.0, represented
     by the URI of the XPath 1.0 specification:
     http://www.w3.org/TR/1999/REC-xpath-19991116.

  * suppressJoinFailure. This attribute determines whether the joinFailure
     fault will be suppressed for all activities in the process. The effect of
     the attribute at the process level can be overridden by an activity using
     a different value for the attribute. The default for this attribute is
     "no".

  * enableInstanceCompensation. This attribute determines whether the process
    instance as a whole can be compensated by platform-specific means. The
    default for this attribute is "no".

  * abstractProcess. This attribute specifies whether the process being defined
    is abstract (rather than executable). The default for this attribute is
    "no".
*/

tProcess:
  X_OPEN K_PROCESS arbitraryAttributes X_NEXT
  tPartnerLinks_opt
  tPartners_opt
  tVariables_opt
  tCorrelationSets_opt
  tFaultHandlers_opt
  tCompensationHandler_opt
  tEventHandlers_opt
  activity
  X_NEXT X_SLASH K_PROCESS X_CLOSE
;

/*---------------------------------------------------------------------------*/

/*
  The token "activity" can be any of the following:

  Basic activities:
    * <receive>
    * <reply>
    * <invoke>
    * <assign>
    * <throw>
    * <wait>
    * <empty>
    * <terminate>

  Structured activities:
    * <sequence>
    * <switch>
    * <while>
    * <pick>
    * <flow>

  Other activities:
    * <scope>
    * <compensate>
*/

activity:
  tEmpty
| tInvoke
| tReceive
| tReply
| tAssign
| tWait
| tThrow
| tTerminate
| tFlow
| tSwitch
| tWhile
| tSequence
| tPick
| tScope
| tCompensate
;


/******************************************************************************
  PARTNER LINKS
******************************************************************************/

/*
  The services with which a business process interacts are modeled as partner
  links in BPEL4WS. Each partner link is characterized by a partnerLinkType.
  More than one partner link can be characterized by the same partnerLinkType.
  For example, a certain procurement process might use more than one vendor
  for its transactions, but might use the same partnerLinkType for all vendors.

  <partnerLinks>
    <partnerLink name="ncname" partnerLinkType="qname" 
             myRole="ncname"? partnerRole="ncname"?>+
    </partnerLink>
  </partnerLinks>
*/

tPartnerLinks_opt:
  /* empty */
| tPartnerLinks X_NEXT
;

tPartnerLinks:
  K_PARTNERLINKS arbitraryAttributes X_NEXT
  tPartnerLink_list //1-oo
  X_SLASH K_PARTNERLINKS
;

tPartnerLink_list:
  tPartnerLink X_NEXT
| tPartnerLink X_NEXT tPartnerLink_list
;

tPartnerLink:
  K_PARTNERLINK arbitraryAttributes X_NEXT X_SLASH K_PARTNERLINK
| K_PARTNERLINK arbitraryAttributes X_SLASH
;


/******************************************************************************
  PARTNERS
******************************************************************************/

/*
  While a partner link represents a conversational relationship between two
  partner processes, relationships with a business partner in general require
  more than a single conversational relationship to be established. To
  represent the capabilities required from a business partner, BPEL4WS uses the
  partner element. 

  <partners>
    <partner name="ncname">+
      <partnerLink name="ncname"/>+
    </partner>
  </partners>
*/

tPartners_opt:
  /* empty */
| tPartners
;

tPartners:
  K_PARTNERS arbitraryAttributes X_NEXT
  tPartner_list // 1-oo
  X_SLASH K_PARTNERS
;

tPartner_list:
  tPartner X_NEXT
| tPartner X_NEXT tPartner_list
;

tPartner:
  K_PARTNER arbitraryAttributes X_NEXT
  tPartnerLink_list // 1-oo
  X_SLASH K_PARTNER
;







/******************************************************************************
  FAULT HANDLERS
******************************************************************************/

/*
  Fault handling in a business process can be thought of as a mode switch from
  the normal processing in a scope. Fault handling in BPEL4WS is always treated
  as "reverse work" in that its sole aim is to undo the partial and
  unsuccessful work of a scope in which a fault has occurred. The completion of
  the activity of a fault handler, even when it does not rethrow the fault
  handled, is never considered successful completion of the attached scope and
  compensation is never enabled for a scope that has had an associated fault
  handler invoked.

  <faultHandlers>?
    <!-- there must be at least one fault handler or default -->
    <catch faultName="qname"? faultVariable="ncname"?>*
      activity
    </catch>
    <catchAll>?
      activity
    </catchAll>
  </faultHandlers>
*/

tFaultHandlers_opt:
  /* empty */
| tFaultHandlers X_NEXT
;

tFaultHandlers:
  K_FAULTHANDLERS arbitraryAttributes X_NEXT
  tCatch_list // 0-oo
  tCatchAll_opt
  X_SLASH K_FAULTHANDLERS
| K_FAULTHANDLERS arbitraryAttributes X_SLASH
;

tCatch_list:
  /* empty */
| tCatch X_NEXT tCatch_list
;

tCatch:
  K_CATCH arbitraryAttributes X_NEXT
  activity X_NEXT // was: tActivityOrCompensateContainer
  X_SLASH K_CATCH
;

tCatchAll_opt:
  /* empty */
| tCatchAll X_NEXT
;

tCatchAll:
  K_CATCHALL arbitraryAttributes X_NEXT
  activity X_NEXT // tActivityOrCompensateContainer
  X_SLASH K_CATCHALL
;


/******************************************************************************
  EVENT HANDLERS
******************************************************************************/

/*
  The whole process as well as each scope can be associated with a set of event
  handlers that are invoked concurrently if the corresponding event occurs. The
  actions taken within an event handler can be any type of activity, such as
  sequence or flow, but invocation of compensation handlers using the
  <compensate/> activity is not permitted.

  <eventHandlers>?
    <!-- Note: There must be at least one onMessage or onAlarm handler. -->
    <onMessage partnerLink="ncname" portType="qname"
               operation="ncname" variable="ncname"?>
      <correlations>?
        <correlation set="ncname" initiate="yes|no"?>+
      <correlations>
      activity
    </onMessage>
    <onAlarm for="duration-expr"? until="deadline-expr"?>*
      activity
    </onAlarm>
  </eventHandlers>
*/

tEventHandlers_opt:
  /* empty */
| tEventHandlers
;

tEventHandlers:
  K_EVENTHANDLERS arbitraryAttributes X_NEXT
  tOnMessage_list // 0-oo
  tOnAlarm_list // 0-oo
  X_SLASH K_EVENTHANDLERS
| K_EVENTHANDLERS arbitraryAttributes X_SLASH
;

tOnMessage_list:
  /* empty */
| tOnMessage
;

tOnAlarm_list:
  /* empty */
| tOnAlarm
;

tOnMessage:
  K_ONMESSAGE arbitraryAttributes X_NEXT
  tCorrelations_opt 
  activity
  X_SLASH K_ONMESSAGE
;

tOnAlarm:
  K_ONALARM arbitraryAttributes X_NEXT
  activity X_NEXT // was: tActivityContainer
  X_SLASH K_ONALARM
;


/******************************************************************************
  COMPENSATION HANDLERS
******************************************************************************/

/*
  Scopes can delineate a part of the behavior that is meant to be reversible
  in an application-defined way by a compensation handler. Scopes with
  compensation and fault handlers can be nested without constraint to arbitrary
  depth.

  <compensationHandler>?
    activity
  </compensationHandler>
*/

tCompensationHandler_opt:
  /* empty */
| tCompensationHandler X_NEXT
;

tCompensationHandler:
  K_COMPENSATIONHANDLER arbitraryAttributes X_NEXT
  activity X_NEXT // was: tActivityOrCompensateContainer
  X_SLASH K_COMPENSATIONHANDLER
;







/******************************************************************************
  VARIABLES
******************************************************************************/

/*
  Business processes specify stateful interactions involving the exchange of
  messages between partners. The state of a business process includes the
  messages that are exchanged as well as intermediate data used in business
  logic and in composing messages sent to partners.
  Variables provide the means for holding messages that constitute the state
  of a business process. The messages held are often those that have been
  received from partners or are to be sent to partners.Variables can also hold
  data that are needed for holding state related to the process and never
  exchanged with partners.

  <variables>
    <variable name="ncname" messageType="qname"?
                type="qname"? element="qname"?/>+
  </variables>
*/

tVariables_opt:
  /* empty */
| tVariables X_NEXT
;

tVariables:
  K_VARIABLES arbitraryAttributes X_NEXT
  tVariable_list // 1-oo
  X_SLASH K_VARIABLES
;

tVariable_list:
  tVariable X_NEXT
| tVariable X_NEXT tVariable_list
;

tVariable:
  K_VARIABLE arbitraryAttributes X_NEXT X_SLASH K_VARIABLE
| K_VARIABLE arbitraryAttributes X_SLASH
;


/******************************************************************************
  CORRELATION SETS
******************************************************************************/

/*
  Each correlation set in BPEL4WS is a named group of properties that, taken
  together, serve to define a way of identifying an application-level
  conversation within a business protocol instance. A given message can carry
  multiple correlation sets. After a correlation set is initiated, the values
  of the properties for a correlation set must be identical for all the
  messages in all the operations that carry the correlation set and occur
  within the corresponding scope until its completion. The semantics of a
  process in which this consistency constraint is violated is undefined.
  Similarly undefined is the semantics of a process in which an activity
  with the initiate attribute set to no attempts to use a correlation set
  that has not been previously initiated.

  <correlationSets>?
    <correlationSet name="ncname" properties="qname-list"/>+
  </correlationSets>
*/

tCorrelationSets_opt:
  /* empty */
| tCorrelationSets X_NEXT
;

tCorrelationSets:
  K_CORRELATIONSETS arbitraryAttributes X_NEXT
  tCorrelationSet_list //1-oo
  X_SLASH K_CORRELATIONSETS
;

tCorrelationSet_list:
  tCorrelationSet X_NEXT
| tCorrelationSet X_NEXT tCorrelationSet_list
;

tCorrelationSet:
  K_CORRELATIONSET arbitraryAttributes X_NEXT X_SLASH K_CORRELATIONSET
| K_CORRELATIONSET arbitraryAttributes X_SLASH
;


/******************************************************************************
  CORRELATIONS
******************************************************************************/

tCorrelations_opt:
  /* empty */
| tCorrelations X_NEXT
;
tCorrelations:
  K_CORRELATIONS arbitraryAttributes X_NEXT
  tCorrelation_list //1-oo
  X_SLASH K_CORRELATIONS
;

tCorrelation_list:
  tCorrelation X_NEXT
| tCorrelation X_NEXT tCorrelation_list
;

tCorrelation:
  K_CORRELATION arbitraryAttributes X_NEXT X_SLASH K_CORRELATION
| K_CORRELATION arbitraryAttributes X_SLASH
;







/******************************************************************************
  EMPTY
******************************************************************************/

/*
  The <empty> construct allows you to insert a "no-op" instruction into a
  business process. This is useful for synchronization of concurrent
  activities, for instance.

  <empty standard-attributes>
    standard-elements
  </empty>
*/

tEmpty:
  K_EMPTY arbitraryAttributes X_NEXT
  standardElements
  X_SLASH K_EMPTY
| K_EMPTY arbitraryAttributes X_SLASH
;


/******************************************************************************
  INVOKE
******************************************************************************/

/*
  The <invoke> construct allows the business process to invoke a one-way or
  request-response operation on a portType offered by a partner.

  <invoke partnerLink="ncname" portType="qname" operation="ncname"
          inputVariable="ncname"? outputVariable="ncname"?
          standard-attributes>
    standard-elements
    <correlations>?
    <catch>*
    <catchAll>?
    <compensationHandler>?
  </invoke>
*/

tInvoke:
  K_INVOKE arbitraryAttributes X_NEXT
  standardElements
  tCorrelations_opt // was: tCorrelationsWithPattern_opt
  tCatch_list //0-oo
  tCatchAll_opt
  tCompensationHandler_opt
  X_SLASH K_INVOKE
| K_INVOKE arbitraryAttributes X_SLASH
;


/******************************************************************************
  RECEIVE
******************************************************************************/

/*
  The <receive> construct allows the business process to do a blocking wait for
  a matching message to arrive.

  <receive partnerLink="ncname" portType="qname" operation="ncname"
           variable="ncname"? createInstance="yes|no"?
           standard-attributes>
    standard-elements
    <correlations>?
      <correlation set="ncname" initiate="yes|no"?>+
    </correlations>
  </receive>
*/

tReceive:
  K_RECEIVE arbitraryAttributes X_NEXT
  standardElements
  tCorrelations_opt
  X_SLASH K_RECEIVE
| K_RECEIVE arbitraryAttributes X_SLASH
;


/******************************************************************************
  REPLY
******************************************************************************/

/*
  The <reply> construct allows the business process to send a message in reply
  to a message that was received through a <receive>. The combination of a
  <receive> and a <reply> forms a request-response operation on the WSDL
  portType for the process.

  <reply partnerLink="ncname" portType="qname" operation="ncname"
         variable="ncname"? faultName="qname"?
         standard-attributes>
    standard-elements
    <correlations>?
       <correlation set="ncname" initiate="yes|no"?>+
    </correlations>
  </reply>
*/

tReply:
  K_REPLY arbitraryAttributes X_NEXT
  standardElements
  tCorrelations_opt
  X_SLASH K_REPLY
| K_REPLY arbitraryAttributes X_SLASH
;


/******************************************************************************
  ASSIGN
******************************************************************************/

/*
  The <assign> construct can be used to update the values of variables with new
  data. An <assign> construct can contain any number of elementary assignments.
  The syntax of the assignment activity is:

  <assign standard-attributes>
    standard-elements
    <copy>+
      from-spec
      to-spec
    </copy>
  </assign>
*/

tAssign:
  K_ASSIGN arbitraryAttributes X_NEXT
  standardElements
  tCopy_list //1-oo
  X_SLASH K_ASSIGN
;

tCopy_list:
  tCopy X_NEXT
| tCopy X_NEXT tCopy_list
;

tCopy:
  K_COPY arbitraryAttributes X_NEXT
  tFrom X_NEXT
  tTo X_NEXT
  X_SLASH K_COPY
; 

/*
  The from-spec MUST be one of the following forms except for the opaque form
  available in abstract processes:

  <from variable="ncname" part="ncname"?/>
  <from partnerLink="ncname" endpointReference="myRole|partnerRole"/>
  <from variable="ncname" property="qname"/>
  <from expression="general-expr"/>
  <from> ... literal value ... </from>
*/

tFrom:
  K_FROM arbitraryAttributes X_NEXT
  X_SLASH K_FROM
| K_FROM arbitraryAttributes X_SLASH
;

/*
  The to-spec MUST be one of the following forms:

  <to variable="ncname" part="ncname"?/>
  <to partnerLink="ncname"/>
  <to variable="ncname" property="qname"/>  
*/

tTo:
  K_TO arbitraryAttributes X_NEXT
  X_SLASH K_TO
| K_TO arbitraryAttributes X_SLASH
;


/******************************************************************************
  WAIT
******************************************************************************/

/*
  The <wait> construct allows you to wait for a given time period or until a
  certain time has passed. Exactly one of the expiration criteria must be
  specified.

  <wait (for="duration-expr" | until="deadline-expr") standard-attributes>
    standard-elements
  </wait>
*/

tWait:
  K_WAIT arbitraryAttributes X_NEXT
  standardElements
  X_SLASH K_WAIT
| K_WAIT arbitraryAttributes X_SLASH
;


/******************************************************************************
  THROW
******************************************************************************/

/*
  The <throw> construct generates a fault from inside the business process.

  <throw faultName="qname" faultVariable="ncname"? standard-attributes>
    standard-elements
  </throw>
*/

tThrow:
  K_THROW arbitraryAttributes X_NEXT
  standardElements
  X_SLASH K_THROW
| K_THROW arbitraryAttributes X_SLASH
;


/******************************************************************************
  COMPENSATE
******************************************************************************/

/*
  The <compensate> construct is used to invoke compensation on an inner scope
  that has already completed normally. This construct can be invoked only from
  within a fault handler or another compensation handler.

  <compensate scope="ncname"? standard-attributes>
    standard-elements
  </compensate>
*/

tCompensate:
  K_COMPENSATE arbitraryAttributes X_NEXT
  standardElements
  X_SLASH K_COMPENSATE
| K_COMPENSATE arbitraryAttributes X_SLASH
;


/******************************************************************************
  TERMINATE
******************************************************************************/

/*
  The terminate activity can be used to immediately terminate the behavior of a
  business process instance within which the terminate activity is performed.
  All currently running activities MUST be terminated as soon as possible
  without any fault handling or compensation behavior.

  <terminate standard-attributes>
    standard-elements
  </terminate>
*/

tTerminate:
  K_TERMINATE arbitraryAttributes X_NEXT
  standardElements
  X_SLASH K_TERMINATE
| K_TERMINATE arbitraryAttributes X_SLASH
;


/******************************************************************************
  FLOW
******************************************************************************/

/*
  The <flow> construct allows you to specify one or more activities to be
  performed concurrently. Links can be used within concurrent activities to
  define arbitrary control structures.

  <flow standard-attributes>
    standard-elements
    <links>?
      <link name="ncname">+
    </links>
    activity+
  </flow>
*/

tFlow:
  K_FLOW arbitraryAttributes X_NEXT
  standardElements
  tLinks_opt
  activity_list //1-oo
  X_SLASH K_FLOW
;

activity_list:
  activity X_NEXT
| activity X_NEXT activity_list
;

tLinks_opt:
  /* empty */
| tLinks X_NEXT
;

tLinks:
  K_LINKS arbitraryAttributes X_NEXT
  tLink_list // 1-oo
  X_SLASH K_LINKS
;

tLink_list:
  tLink X_NEXT
| tLink X_NEXT tLink_list
;

tLink:
  K_LINK arbitraryAttributes X_NEXT X_SLASH K_LINK
| K_LINK arbitraryAttributes X_SLASH
;


/******************************************************************************
  SWITCH
******************************************************************************/

/*
  The <switch> construct allows you to select exactly one branch of activity
  from a set of choices.

  <switch standard-attributes>
    standard-elements
    <case condition="bool-expr">+
      activity
    </case>
    <otherwise>?
      activity
    </otherwise>
  </switch>
*/

tSwitch:
  K_SWITCH arbitraryAttributes X_NEXT
  standardElements
  tCase_list //1-oo
  tOtherwise_opt
  X_SLASH K_SWITCH
;

tCase_list:
  tCase X_NEXT
| tCase X_NEXT tCase_list
;

tCase:
  K_CASE arbitraryAttributes X_NEXT
  activity X_NEXT // was: tActivityContainer
  X_SLASH K_CASE
;

tOtherwise_opt:
  /* empty */
| tOtherwise X_NEXT
;

tOtherwise:
  K_OTHERWISE arbitraryAttributes X_NEXT
  activity X_NEXT // was: tActivityContainer
  X_SLASH K_OTHERWISE
;


/******************************************************************************
  WHILE
******************************************************************************/

/*
  The <while> construct allows you to indicate that an activity is to be
  repeated until a certain success criteria has been met.

  <while condition="bool-expr" standard-attributes>
     standard-elements
     activity
  </while>
*/

tWhile:
  K_WHILE arbitraryAttributes X_NEXT
  standardElements
  activity X_NEXT
  X_SLASH K_WHILE
;


/******************************************************************************
  SEQUENCE
******************************************************************************/

/*
  The <sequence> construct allows you to define a collection of activities to
  be performed sequentially in lexical order.

  <sequence standard-attributes>
    standard-elements
    activity+
  </sequence>
*/

tSequence:
  K_SEQUENCE arbitraryAttributes X_NEXT
  standardElements
  activity_list //1-oo
  X_SLASH K_SEQUENCE
;


/******************************************************************************
  PICK
******************************************************************************/

/*
  The <pick> construct allows you to block and wait for a suitable message to
  arrive or for a time-out alarm to go off. When one of these triggers occurs,
  the associated activity is performed and the pick completes.

  <pick createInstance="yes|no"? standard-attributes>
    standard-elements
    <onMessage partnerLink="ncname" portType="qname"
               operation="ncname" variable="ncname"?>+
      <correlations>?
         <correlation set="ncname" initiate="yes|no"?>+
      </correlations>
      activity
    </onMessage>
    <onAlarm (for="duration-expr" | until="deadline-expr")>*
      activity
    </onAlarm>
  </pick>

*/

tPick:
  K_PICK arbitraryAttributes X_NEXT
  standardElements
  tOnMessage X_NEXT tOnMessage_list //1-oo
  tOnAlarm_list //0-oo
  X_SLASH K_PICK
;







/******************************************************************************
  SCOPE
******************************************************************************/

/*
  The <scope> construct allows you to define a nested activity with its own
  associated variables, fault handlers, and compensation handler.

  <scope variableAccessSerializable="yes|no" standard-attributes>
    standard-elements

    <variables>?
    <correlationSets>?
    <faultHandlers>?
    <compensationHandler>?
    <eventHandlers>?

    activity
  </scope>
*/

tScope:
  K_SCOPE arbitraryAttributes X_NEXT
  standardElements
  tVariables_opt
  tCorrelationSets_opt
  tFaultHandlers_opt
  tCompensationHandler_opt
  tEventHandlers_opt
  activity
  X_SLASH K_SCOPE
;







/******************************************************************************
  STANDARD ELEMENTS
******************************************************************************/

/*
  Note that the "standard-elements" referred to above are:

  <target linkName="ncname"/>*
  <source linkName="ncname" transitionCondition="bool-expr"?/>*

  where the default value of the "transitionCondition" attribute is "true()",
  the truth-value function from the default expression language XPath 1.0.
*/

standardElements:
  tTarget_list //0-oo
  tSource_list //0-oo
;

tSource_list:
  /* empty */
| tSource X_NEXT tSource_list
;

tSource:
  K_SOURCE arbitraryAttributes X_NEXT X_SLASH K_SOURCE
| K_SOURCE arbitraryAttributes X_SLASH
;

tTarget_list:
  /* empty */
| tTarget X_NEXT tTarget_list
;

tTarget:
  K_TARGET arbitraryAttributes X_NEXT X_SLASH K_TARGET
| K_TARGET arbitraryAttributes X_SLASH
;


/*---------------------------------------------------------------------------*/

arbitraryAttributes:
  /* empty */
| X_NAME X_EQUALS X_STRING arbitraryAttributes
;
