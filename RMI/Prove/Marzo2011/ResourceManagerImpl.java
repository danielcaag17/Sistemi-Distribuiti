package RMI.Prove.Marzo2011;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.LinkedList;
import java.util.Queue;

public class ResourceManagerImpl extends UnicastRemoteObject implements ResourceManager {
    private boolean isResourceInUse;
    private final Queue<String> queue;

    public ResourceManagerImpl() throws RemoteException {
        super();
        isResourceInUse = false;
        queue = new LinkedList<>();
    }

    @Override
    public boolean requestAccess(String clientId) throws RemoteException {
        if (isResourceInUse) {
            queue.add(clientId);    // Client in the queue
            return false;
        }
        else {
            isResourceInUse = true; // Client is using the service
            return true;
        }
    }

    @Override
    public void releaseAccess(String clientId) throws RemoteException {
        isResourceInUse = false;
        if (!queue.isEmpty()) {
            queue.poll();           // Another client is using the service
            isResourceInUse = true;
        }
    }
}
