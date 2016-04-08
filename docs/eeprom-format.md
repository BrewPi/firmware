
# Eeprom Format

```
      0x00	magic number (0x69)
	  0x01  version (currently 0x01)
	  0x02	active profile (0xFF for no profile, else a 0-based id for the profile.)
	  0x03	system id (0xFF unspecified)
	  0x04  end of open profile - if this is equal to the start of the profile then it needs to be computed and written.
	  0x06	start of profile FAT. 2 bytes per entry. This lists the start address of the profiles.
	  0x0E	start of profile storage
	  ....
	  end
```

