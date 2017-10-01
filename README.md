# Smart Outlet
The purpose of this project is to use a ESP8266-backed [WeMos](http://www.wemos.cc) chip and a 2-relay interface board inside an extra-deep single-gang box to create a smart electrical outlet. 

## Background
I occasionally mine bitcoin (when I have electricity included in my rent). My bitcoin miner uses 1300 watts—that's a lot!

<img src="img/bitcoin_miner.jpg" width="400">

The miner has two power supplies, each drawing nearly 5.5 amps. When plugging in the machine, the top power supply must be plugged in first, followed by the bottom power supply within three seconds. This is to allow the top power supply to power on the beaglebone controller and one of the mining boards and give them time to be ready for the second mining board to come online. This gave me the idea to use an arduino chip that would, when given the signal, power on one outlet and the second one within three seconds. 

I also have a [Nest Learning Thermostat](https://nest.com/thermostat/meet-nest-thermostat/), which offers an API and wireless connectivity. Since my bitcoin miner sounds like a vaccum cleaner and puts off the heat of a space heater, it seemed like a cool project to have my bitcoin miner and my Nest thermostat talk to each other and coordinate the heating/cooling of my apartment. In my mind, it would go something like this:
* Nest has learned that I usually leave my apartment at 8:45am. It turns off the A/C. 
* My bitcoin miner is notified that the A/C is off and it's ok to heat up the apartment by mining bitcoin. It powers up.
* Nest knows that I usually get home around 6pm and that it can cool my apartment at a rate of x degrees per hour. It figures out what time to turn on the A/C to begin cooling, and powers on then.
* My bitcoin miner is notified that the A/C is on, and that it should stop mining so that it's not fighting the A/C. It powers off.

At my work, we have been learning about arduino boards and building robots in a mechatronics group brown-bag lunch every Friday. I ordered some ESP8266-backed [WeMos](http://www.wemos.cc) chips because they looked like the perfect chip for my idea. I could hook that up to a 2-channel relay interface board to turn on and off the two independently-switched outlets on my single-gang electrical box.

## Tools & Supplies
* Liquid Tape

<img src="img/liquid_tape.jpg" width="200">

* 2-channel relay interface board from [SainSmart](http://www.sainsmart.com/sainsmart-2-channel-5v-relay-module-for-arduino-raspberry-pi.html)

<img src="img/2-channel-relay.jpg" width="200">

* [WeMos D1-mini](https://wiki.wemos.cc/products:d1:d1_mini) (Two are pictured here for a top & bottom view)

<img src="img/wemos.jpg" width="200">

* a [HLK-PM01 AC-DC 220V to 5V Step-Down Power Supply Module Intelligent Household Switch Power Supply Module](http://www.aliexpress.com/item/10pcs-HLK-PM01-AC-DC-220V-to-5V-Step-Down-Power-Supply-Module-Intelligent-Household-Switch/32320072459.html)

<img src="img/converter.jpg" width="200">

* a power cable/extension cord capable of handling at least 15 amps—I used a computer power cable that was forked like this:

<img src="img/power_cable.jpg" width="200">

* an extra-deep single gang electrical box and clamp connector (I bought at Home Depot)

<img src="img/gang-box-and-clamp-connector.jpg" width="200">

* a cheap electrical outlet

<img src="img/outlet.png" width="200">

* solder
* soldering iron
* wire cutters
* wire strippers
* phillips screw driver
* pliars
* multimeter
* 2 3-volt AA battery packs
* various jumper wires
* 6V, 50mA miniature lamp

## Software
* Arduino IDE or C Lion & Platformio
* `smart-outlet.cpp`

# Steps
## Step 1: Prepare the power cable
* Cut off the end of the power cable that is farthest from where you'd plug it in the wall
* Screw in the clamp connector to the hole in the top of the gang box
* Feed the cut-off end through the hole and clamp connector in the gang box, from the outside of the box toward the inside.
* Push the gang box all the way to the end of the power cable (the end where you plug it in to the wall). This will give you room to work on the other end of the cable.

<img src="img/box-at-end.jpg" width="400">

* Strip off the cable shielding to reveal the hot, neutral, and ground wires. Use the continuity function of the multimeter to verify which wire is hot, which is neutral, and which is ground. In my case, the green wire is ground, the blue wire is neutral, and the brown wire is hot. You use this diagram to make sure you get it right:

<img src="img/wiring-diagram.jpg" width="600">

* There's a brass tab on each side of the outlet: the left side tab connects the neutral terminal for the top outlet and the neutral terminal for the bottom outlet, while the right side tab connects the hot terminal for the top outlet and the hot terminal for the bottom outlet. This causes the outlets to be switched together. Since we want the two outlets to be powered independently, we want to break the tab off on the right (hot) side. Grab right tab with pliars and bend it back and forth until it breaks off. Leave the left (neutral) tab intact.

## Step 2: Test the relays
* Checkout the diagram for the relays:

<img src="img/relay-diagram.jpg" width="700">

* On the right side of the diagram, we see two yellow boxes with numbers 1, 2, and 3.
* These yellow boxes correspond to the two relay terminals (the blue strip of plastic with screws in it, labeled K1 and K2)
* There are three lines coming out of the yellow boxes that go down. If we follow them, we see that line 2 and line 3 are connected, with line 1 disconnected. Relays work by applying a signal to an electromagnet and connecting (here) lines 1 and 2. Think of the end of line 2 as being on a pivot that normally rests connecting lines 2 and 3, and the little blue box next to line 1 as being an electromagnet that (when a signal is applied) pulls on the end of line 2, which pivots away from line 3.
* The yellow boxes on the left side of the diagram correspond to the inputs to the relay board. The bottom yellow box shows line 4 is VCC, or 5V in, and line 1 is ground. Line 3 is signal input 1 (for relay K1) and line 2 is signal input 2 (for relay K2). Signal inputs 1 and 2 will come from the WeMos digital out pins. 
* The question remains, however: does a high signal in or a low signal in cause the relay to switch? We should test it! 
* Here I have a 6-volt 50mA Miniature Lamp and two 3-volt AA battery packs hooked up in series to produce 6V. We'll use the batteries to power the lamp. Notice that the lamp blue lead is common/ground, and the lamp white lead is VCC.

<img src="img/lamp.jpg" width="700">

* We'll add in the relay in its default, powered-off state, and connect the blue lamp lead to ground and the white lamp lead to diagram terminal 3. We add a white jumper wire to diagram terminal 2 and to VCC of the batteries. We see that the lamp is powered on, verifying that diagram terminals 3 and 2 are normally connected, closing the circuit.

<img src="img/lamp-relay-1.jpg" width="700">

* Now we remove the white lamp lead from its relay terminal and connect it to diagram terminal 1. We also remove the jumper wire from diagram terminal 3 and connect it to diagram terminal 2. We see that the lamp is off, verifying that diagram terminals 1 and 2 are normally disconnected, leaving that circuit open.

<img src="img/lamp-relay-2.jpg" width="700">

* Now we want to verify the behavior of the relay. We will power the relay board with the battery packs as well (I probably shouldn't have put 6 volts through a 5V relay board, but I did. Luckily it didn't burn up). We leave the lamp as it is currently connected (this is the setup we want: the circuit will be normally open—diagram terminals 1 and 2 are disconnected—then we apply a signal to connect 1 and 2, thus closing the circuit). We connect a jumper wire from the battery VCC to VCC in on the relay board (the bottom left yellow box on the diagram, line 4). We also connect a jumper wire from the battery ground to the ground on the relay board (the bottom left yellow box on the diagram, line 1). Lastly, we connect a jumper wire from battery ground to signal input 1 (the bottom left yellow box on the diagram, line 3). You should hear a click and see the lamp turn on! Removing the signal should also click and the lamp should turn off. This was just a guess—a low signal activates the relay. I'm not an electrical engineer and don't know how to read the complete diagram (though I'm learning!). We could have just as easily connected the battery VCC to signal 1 input; we would have seen nothing happen and tried connecting signal 1 input to ground, which would show the correct behavior.

<img src="img/lamp-relay-3.jpg" width="700">

<img src="img/lamp-relay-4.jpg" width="700">

## Step 3: Connect Power Cable to Relay and Outlet
* We will need to splice the power cable: the neutral will need to be spliced into 2, and the hot will need to be spliced into 3, and the ground will need to be spliced into 2.
* One neutral splice will be connected to the left side of the electrical outlet (the side with the brass tab intact), and the other will be connected to the AC/DC converter module. 
* One ground splice will be connected to the green screw at the bottom of the electrical outlet, and the other will be connected to the green screw at the bottom back of the gang box.
* One hot splice will be connected to the top electrical outlet terminal on the right side, another will be connected to the bottom electrical outlet on the right side, and the third splice will be connected to the AC/DC converter module.
* Cut off some of the power cable to give you extra wires to splice.
* Hook the wires that will be spliced around one another to provide mechanical strength, then solder them together. 
* Note: in these pictures, I made the mistake of breaking off both brass tabs on the outlet, so I needed to splice the neutral wire into 3 as well. 

<img src="img/splice-1.jpg" width="700">

<img src="img/splice-2.jpg" width="700">

* Here you can see where the neutral wire comes from the power cable, is spliced into 3 (or two, if you left the brass tab intact for the neutral side), and is connected to the left side of the outlet, with one wire left over for the AC/DC converter.

<img src="img/splice-3.jpg" width="700">

* In this image below, the outlet is upside down. We can see how we want the hot splices to be hooked up to the relay and the outlet. Normally, we want the outlet to be connected to nothing, so we connect the top (bottom in the picture) outlet hot wire to relay K2's terminal 2, and leave terminal 3 disconnected. Then when we apply a low signal to relay K2, it will connect K2's terminal 2 to K2's terminal 1. So that means we need to connect K2's terminal 1 to the hot wire coming from the power cable. The process is the same for relay K1: we connect the bottom (top in the picture) outlet's hot terminal to K1's terminal 2 and the power cable's hot splice to K1's terminal 1, thus leaving K1's terminal 3 disconnected.

<img src="img/splice-4.jpg" width="700">

* Another angle of the outlet after splicing:

<img src="img/splice-5.jpg" width="700">

<img src="img/splice-6.jpg" width="700">

* And after soldering:

<img src="img/splice-7.jpg" width="700">

* I find it easier to apply a couple coats of liquid tape with the wires up in the air.

<img src="img/splice-8.jpg" width="700">

<img src="img/splice-9.jpg" width="700">

## Step 4: Connect AC/DC Converter to Wemos and relay
* Since our incoming voltage is 120VAC, and both our relay board and the wemos operate at 5v, we connect the AC wires to the AC/DC converter, which outputs 5v. We solder it and apply liquid tape. Also, we connect jumper wires to the 5v out pins. Here, I used purple for positive and blue for negative.

<img src="img/converter2.jpg" width="700">

* Now the purple and blue wires will need to be spliced, since both the relay board and the wemos board need 5v to operate. Splice, solder, and apply liquid tape.

<img src="img/5v-splice.jpg" width="700">

* Connect the 5v wires to the appropriate pins on both boards. Here we see in the first image how the positive 5v purple wire plugs in to VCC on the relay board, and the blue negative wire plugs in to the ground pin on the relay board. Also show are the yellow and orange wires, which are the signal wires for relay 1 and relay 2. In the images we can also see how the red wire is spliced from the purple wire, and it plugs in to the wemos vcc pin, while the black wire splices from the blue wire and plugs in to the wemos ground pin. The yellow and orange wires connect the wemos digital I/O pins D1 and D2 to the signal in 1 and signal in 2 pins on the relay board. For reasons I'll cover later, switch the orange D2 pin to D3 instead.

<img src="img/relay-wires.jpg" width="700">

<img src="img/wemos-wires.jpg" width="700">

## Step 5: Shove everything into the electrical box

* We should give things an extra coat of electrical tape so that they don't short against the metal electrical box:

<img src="img/insulate.jpg" width="700">

* It's kind of tricky to fit everything in to the box. I bent off the top and bottom metal brackets of the outlet, since they aren't being used. Only the single screw portion of the bracket remains on the top and bottom of the outlet. 

<img src="img/bracket.jpg" width="700">

* Also, I swapped the wemos out for one that I only solder on the pin housing to one side of the wemos board. That way it fits better. Also, the wires that connect to the pin housing use 90-degree angled pins so that they fit better and don't bump in to the body of the outlet. I used some foam to insulate both sides of the wemos, and I plugged in a small usb cable to the bottom of the wemos (not pictured here). I fed the end of the usb cable through the bottom of the electrical box, where it can be easily accessed for code updates. I also oriented the wemos chip so that the wireless antenna is pointed toward the faceplate of the outlet. That way it can get better wifi signal and you can also faintly see the built-in blue LED light on the wemos from the outside of the box.

<img src="img/foam.jpg" width="700">

<img src="img/angle-pins.jpg" width="700">

<img src="img/tight.jpg" width="700">

## Step 5: Close up the box
* Now we are ready to close up the box. Make sure the USB cable is accessible from the bottom of the box. Screw down the bracket holding the cable going into the top of the box. Screw on the faceplate.

<img src="img/screw-in.jpg" width="700">

<img src="img/usb.jpg" width="700">

<img src="img/closed-box.jpg" width="700">

<img src="img/upload-code.jpg" width="700">

## Step 6: Code!
* I shoved the USB cable in the box, just in case I botch the OTA code uploads. 
* I have C-Lion by Intellij, which I love because I also use Intellij IDEA at work and Webstorm for my web projects. I like the environment, especially with the VIM plugin.
* I use platformio to set everything up.
* Begin by cloning this repository, or just download the src folder if you don't want all these images and markdown included.
* Install [Platformio](http://docs.platformio.org/en/latest/installation.html) command line tool
* I just used the `brew install platformio` command since I'm on a mac. It worked beautifully!
* `cd` in to the repo directory (or the src directory), and use `platformio init --ide clion --board huzzah`
* Now you can open in CLion and use the preconfigured Build configurations to build and upload your code
* In order for the Wemos to be recognized on the computer, make sure to install any necessary drivers (see the [Wemos installation page](http://www.wemos.cc/downloads/))
* Go ahead and use the Build functionality in CLion to build and upload your code. Note that the "Run" button doesn't work. Just use the menu item `Run` > `Build` on the selected configuration.

