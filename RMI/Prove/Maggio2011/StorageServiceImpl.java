package RMI.Prove.Maggio2011;

import java.rmi.*;
import java.rmi.server.*;
import java.util.HashMap;
import java.util.Map;

public class StorageServiceImpl extends UnicastRemoteObject implements StorageService {
    private Map<String, String> services;
    
    public StorageServiceImpl() throws RemoteException {
        super();
        this.services = new HashMap<>();
    }

    @Override
    public void registry(String nameService, String nameServer) throws RemoteException {
        services.put(nameServer, nameService);        
    }

    @Override
    public String getNameService(String nameServer) throws RemoteException {
        return services.get(nameServer);
    }    
}
