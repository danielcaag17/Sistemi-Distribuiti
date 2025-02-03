import jade.core.Agent;

public class BookBuyerAgent extends Agent { // In order to create a JADE agent
    protected void setup() {    // Method to include agent initializations, the agent job is tipically carried out within "behaviors"
        // Printout a welcome message
        System.out.println("Hello World! My name is " + getLocalName());
    
        // Make this agent terminate
        doDelete();
        // Even if it does not have anything else to do after printing the welcome message, the agent is still running. In order to make it terminate doDelete() must be called.
    }

    // Similarly to the setup() that is invoked by JADE, the takeDown() is invoked just before an agent terminates to include agent clean up operations.
    protected void takeDown() {
        System.out.println("Agent " + getLocalName() + ": terminating");
    }
}

/*
 * A behaviour represents a task that an agent can carry out and is implemented as an object of a class that extends jade.core.bejaviours.Behaviour
 * Each class extending Behavior must implement the action() method, defines the operations to be performed
 * and the done() method, returns a boolean that specifies whether or not a behaviour has completed and have to be removed from the pool of bejaviours an agent is carrying out.
 */

// This class does not extends Behaviour but CyclicBehaviour due to the fact that must be continuously running to check if a message has benn received.
public class OverbearingBehaviour extends CyclicBehaviour {
    public void action() {
        MessageTemplate mt = MessageTemplate.MatchPerformactive(ALCMessage.CFP);
        ACLMessage msg = myAgent.recieve(mt);
        if (msg != null) {
            // CFP Message reveived. Process it
            // CFP means Call For Proposal, this is an ACL message parameter (the communicative intention also called "performative")
            // ACLMessage.setPerformative(ACLMessage.CFP);
            String msgContent = msg.getContent();
            ACLMessage reply = msg.createReply();

            // reply.setPerformative(ACLMessage.CFP);   //There are other possible parameters
            // reply.setContent("This is the content");
            myAgent.send(reply);
        } else {
            /*
            * The agent's thread starts a continuous loop that is extremly CPU consuming.
            * To avoid that we would like to execute the action() method only when a new message is received.
            * It is possible to use the block() method. This marks the behaviour as "blocked" so that the agent does not schedule it for action() method.
            * When a new message is inserted in the agent's message queue all blocked behaviours becomes available for execution again.
            */
            block();
        }
    }

    public boolean done() {
        return true;
    }
}

// How to implement a message in JADE
/*
 * ACLMessage msg = new ACLMessage(ACLMessage.INFORM);
 * msg.addReceiver(new AID("Peter", AID.ISLOCALNAME));
 * msg.setLanguage("English");
 * msg.setOntology("Weather-forecast-ontology");
 * msg.setContent("Today it's raining");
 * seng(msg);
 */

