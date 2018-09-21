import re
import gdb


def lookup_function(val):
    "Look-up and return a pretty-printer that can print val."
    # Get the type.
    type = val.type

    # If it points to a reference, get the reference.
    if type.code == gdb.TYPE_CODE_REF:
        type = type.target()

    # Get the unqualified type, stripped of typedefs.
    type = type.unqualified().strip_typedefs()

    # Get the type name.
    typename = type.tag      
    
    if typename == None:
        return None
    
    # Iterate over local dictionary of types to determine
    # if a printer is registered for that type.  Return an
    # instantiation of the printer if found.
    for function in sorted(pretty_printers_dict):
        if function.match(typename):
            return pretty_printers_dict[function](val)

    # Cannot find a pretty printer.  Return None.
    return None

class FixedPointPrinter:
    "Pretty Printer for safe_elastic_fixed_point"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        raw = self.val['_rep']['_rep']['_rep']
        m = re.search('cnl::_impl::number_base<cnl::fixed_point<cnl::overflow_integer<cnl::elastic_integer<([0-9]+), (.+)>,\s*cnl::saturated_overflow_tag>,\s*(-[0-9]+),\s*([0-9])>,\s*cnl::overflow_integer<cnl::elastic_integer<([0-9]+),\s*(.+)>,\s*cnl::saturated_overflow_tag>\s*>',
                      str(self.val.type))
        scale = 2**(-int(m.group(3)))
        scaled = (raw + 0.0) / scale
        return "{0}: {1}".format(raw, scaled)
        
    def display_hint(self):
        return 'string'   

# register the pretty-printer
pretty_printers_dict={}
pretty_printers_dict[
    re.compile(
        '.*cnl::_impl::number_base<cnl::fixed_point<.*>.*>'   
        )
]=FixedPointPrinter
gdb.pretty_printers.append(lookup_function)

