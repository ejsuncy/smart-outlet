# Smart Outlet
The purpose of this project is to use a ESP8266-backed [WeMos](http://www.wemos.cc) chip and a 2-relay interface board inside an extra-deep single-gang box to create a smart electrical outlet. 

## Background
I occasionally mine bitcoin (when I have electricity included in my rent). My bitcoin miner uses 1300 watts—that's a lot!

<img src="bitcoin_miner.jpg" width="400">

The miner has two power supplies, each drawing nearly 5.5 amps. When plugging in the machine, the top power supply must be plugged in first, followed by the bottom power supply within three seconds. This is to allow the top power supply to power on the beaglebone controller and one of the mining boards and give them time to be ready for the second mining board to come online. This gave me the idea to use an arduino chip that would, when given the signal, power on one outlet and the second one within three seconds. 

I also have a [Nest Learning Thermostat](https://nest.com/thermostat/meet-nest-thermostat/), which offers an API and wireless connectivity. Since my bitcoin miner sounds like a vaccum cleaner and puts off the heat of a space heater, it seemed like a cool project to have my bitcoin miner and my Nest thermostat talk to each other and coordinate the heating/cooling of my apartment. In my mind, it would go something like this:
* Nest has learned that I usually leave my apartment at 8:45am. It turns off the A/C. 
* My bitcoin miner is notified that the A/C is off and it's ok to heat up the apartment by mining bitcoin. It powers up.
* Nest knows that I usually get home around 6pm and that it can cool my apartment at a rate of x degrees per hour. It figures out what time to turn on the A/C to begin cooling, and powers on then.
* My bitcoin miner is notified that the A/C is on, and that it should stop mining so that it's not fighting the A/C. It powers off.

At my work, we have been learning about arduino boards and building robots in a mechatronics group brown-bag lunch every Friday. I ordered some ESP8266-backed [WeMos](http://www.wemos.cc) chips because they looked like the perfect chip for my idea. I could hook that up to a 2-channel relay interface board to turn on and off the two independently-switched outlets on my single-gang electrical box.

## Tools & Supplies
* Liquid Tape

<img src="liquid_tape.jpg" width="200">

* 2-channel relay interface board from [SainSmart](http://www.sainsmart.com/sainsmart-2-channel-5v-relay-module-for-arduino-raspberry-pi.html)

<img src="2-channel-relay.jpg" width="200">

* [WeMos D1-mini](http://www.wemos.cc/Products/d1_mini.html) (Two are pictured here for a top & bottom view)

<img src="wemos.jpg" width="200">

* a [HLK-PM01 AC-DC 220V to 5V Step-Down Power Supply Module Intelligent Household Switch Power Supply Module](http://www.aliexpress.com/item/10pcs-HLK-PM01-AC-DC-220V-to-5V-Step-Down-Power-Supply-Module-Intelligent-Household-Switch/32320072459.html)

<img src="converter.jpg" width="200">

* a power cable/extension cord capable of handling at least 15 amps—I used a computer power cable that was forked like this:

<img src="power_cable.jpg" width="200">

* an extra-deep single gang electrical box and clamp connector (I bought at Home Depot)

<img src="gang-box-and-clamp-connector.jpg" width="200">

* a cheap electrical outlet

<img src="outlet.png" width="200">

* solder
* soldering iron
* wire cutters
* wire strippers
* phillips screw driver
* pliars

##Software
* Arduino IDE or C Lion & Platformio
* `smart-outlet.cpp`



