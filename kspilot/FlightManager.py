import krpc;
import time;

#somethin somethin helper
current_milli_time = lambda: int(round(time.time() * 1000))

#a tasl is something that, if ticed, does something with something. yeah that general. 
class Task:
    def __init__(self):
        self.activated = False 
        self.tickActivated = 0 
    def activate(self, manager):
        self.activated = True 
        self.tickActivated = manager.ticks

    #tickes. gets the manager as argument. Return true if the task is done.
    def tick(self, manager):
        raise NotImplementedError("Please implement tick in the child class of task!")

class Telemetry:
    def __init__(self, rawVessel, conn):
        self.rawVessel = rawVessel
        self.conn = conn
        self.altitude = self.conn.add_stream(getattr, self.rawVessel.flight(), 'mean_altitude')
        self.apoapsis = self.conn.add_stream(getattr, self.rawVessel.orbit, 'apoapsis_altitude')
        self.periapsis  = self.conn.add_stream(getattr, self.rawVessel.orbit, 'periapsis_altitude')
        self.eccentricity = self.conn.add_stream(getattr, self.rawVessel.orbit, 'eccentricity')
        self.resources = dict()
        self.streams = dict()
    def addAdditionalStream(self, name, stream):
        self.streams[name] = stream 
        
    def addResource(self, name, stage, cumulative=False):
        stageResources = self.rawVessel.resources_in_decouple_stage(stage, cumulative)
        resources[name] = conn.add_stream(stageResources.amount, name)
        
class Vessel:
    def __init__(self, rawVessel, conn):
        self.rawVessel = rawVessel 
        self.conn = conn
        self.telemetry = Telemetry(rawVessel, conn)
    def stage(self):
        self.rawVessel.control.activate_next_stage()
        
        
class FlightManager:
    #Initializes the FlightManager. Will then setup the telemetry and then is done
    def __init__(self, conn_name, host, comm_port, s_port):
        self.conn = krpc.connect(address=host,rpc_port=comm_port,stream_port=s_port,name=conn_name)
        self.initRest()
    def __init__(self, conn_name):
        self.conn = krpc.connect(name=conn_name)
        self.initRest()
    def initRest(self):
        self.vessel = Vessel(self.conn.space_center.active_vessel, self.conn)
        self.monitors = list()
        self.tasks = list()
        self.ticks = 0
        self.ticklength = 0.1
    #Adds a monitor. A monitor is a task that is constantly running and not exiting/depending on the flight schedule
    def addMonitor(self,monitor):
        self.monitors.append(monitor)
    def addTask(self,task):
        self.tasks.append(task)
    #exectutes the flight 
    def run(self):
        try:
            if len(self.tasks) > 0:
                self.tasks[0].activate(self)
            currStart = current_milli_time()
            while True:                
                if current_milli_time()-currStart > 100:
                    self.ticks+=1 
                    for m in self.monitors:
                        m.tick(self);
                    if len(self.tasks) > 0:
                        if self.tasks[0].tick(self) == True:
                            self.tasks.pop(0);
                            if len(self.tasks) > 0:
                                self.tasks[0].activate(self)
                    currStart = current_milli_time() 
            print("No more stuff to do. Yay. Are you alive still? Not? Blame the programmer")
        except:
            print("Warning: Exception. Probably a dissconnect or a keyboard Interuppt. Happy Exploding!")
            raise