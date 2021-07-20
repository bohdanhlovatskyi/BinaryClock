- NodeMCU uses some library to fetch the time. It is bad, because it need to hardcode the timezone.

TODO:
- For some reason NodeMCU won't work from 3/5V from Arduino (into Vin pin)
- Rewrite the data fetching
- Software Serial at NodeMCU has a conflict with the common uart pins, therefore the two serial connections do not work (the software one overwrites the classic one) - find out what pins to use