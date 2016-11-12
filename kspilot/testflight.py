from FlightManager import *
from generalMonitors import *
from generalTasks import *

manager = FlightManager("Testflight")
manager.addMonitor(DataMonitor())
manager.addTask(SimpleCountdown())
manager.addTask(KeepUpwards())
manager.run()