import gdb.printing
import re


class TempPrinter:
    "Pretty Printer for temp_t and temp_long_t"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        raw = self.val['value_']
        val = (raw + 0.0) / 256.0
        return "{0} ({1})".format(val, raw)


class TempPrecisePrinter:
    "Pretty Printer for temp_precise_t"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        raw = self.val['value_']
        val = (raw + 0.0) / 16777216.0
        return "{0} ({1})".format(val, raw)


def brewpi_pretty(val):
    lookup_tag = val.type.tag
    if lookup_tag == None:
        return None
    regex = re.compile('^temp_t$')
    if regex.match(lookup_tag):
        return TempPrinter(val)
    regex = re.compile('^temp_long_t$')
    if regex.match(lookup_tag):
        return TempPrinter(val) # same nr of fraction bits
    regex = re.compile('^temp_precise_t$')
    if regex.match(lookup_tag):
        return TempPrecisePrinter(val)

    return None

# Register our pretty-printers with |objfile|.
def register_pretty_printer():
    #gdb.printing.register_pretty_printer(
    #     gdb.current_objfile(),
    #         build_pretty_printer())
    gdb.pretty_printers.append(brewpi_pretty)



register_pretty_printer()


