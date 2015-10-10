python 
import sys 

sys.path.insert(0, 'C:/repos/firmware/tools/prettyprinters') 
import printers

sys.path.insert(0, 'C:/repos/libstdc++-v3-python/python')
from libstdcxx.v6.printers import register_libstdcxx_printers

end 