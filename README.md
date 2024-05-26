# Bluebird-Heatpump-Weheat

Weheat company makes a hybrid heatpump called blackbird. It combines the possibility of using electricity for the heatpump and gas for an additional boiler as the energy source. This means you can get fast heat using the gas and use electrical heating the rest of the time. I have an Anna thermostat which works together with an ADAM HA gateway both from Plugwise. The system was installed in September 2023 and after some teething problems has worked for the most port although there are still some issues to be fixed. <br/>

There are still quirks which i would like to address and I am working to try to do that step by step. <br/>
Firstly the Anna stears the ADAM and this in turn sends the required water temperature to the Bluebird. Unfortunately the ADAM control mechanism has a minimum water temperature setting at 27degrees.
I am told that this is not the case for everyone. Mayber somebody can help me find a setting to overrule this or something.
On some days this temperature is still too much for the requirements of the house. It results in the heatpump being switched off totally. After a while as the room temperature drops the system suddenly tries to start up again and this can even result in gas being used as the heating source. In any case the heatpump switches on and off which is not desired.<br/>

On few occasions my shower which uses the separate boiler stops giving hot water. This is most annoying as you are wet from the shower and not in a position to fix anything.

Then i have a dynamic energy price contract which means the cost for my electricvity changes every hour. Experience learns that the room temperature only changes very slowly when the heatpump is off. (maybe one degree per two hours) This means i could anticipate required room temperatures and heat the room a degree above the setpoint at low energy price while leaving the heatpump off when prices are high.
By managing when the heatpump is switched on and off i can to a certain extent reduce the average electricity price i would pay.

I also have solar pannels and this too could help by adjusting the power the heat pump uses to match the electricity the solar panels produce.

Finally most energy suppliers are starting to help you trade in the "onbalans" market. This means they have control over some of the households energy users and suppliers (eg batteries). Tibber my energy supplier for instance has rewards which is a way for them to manage the charging of my EV car such that there are rewards for stabilizing the electricity network. There is a sharing of the additional profit that brings. It is expected that this will be expanded in the near future. Battery suppliers work with gateways which control the batteries. 
The day ahead trading spot market works with fixed energy prices one day ahead. It therefore has to use forecasts which can be inaccurate. The weather may not be as predicted which can mean wind or solar production is less or more than predicted. This means supply and demand wopuld not be in balans. The Transmission System Operator (TSO) in holland Tennet then sends signal to the BSP (Balancing Service Providers) to provide or consume the balancing energy. This can be the release or storage of batteries or the turing on of heatpumps. It is essential that this is done so that the frequaency 50Hz and the stability of the network is maintained for a safe use of the network by all.
Weheat could easily develop such a gateway to anable control of their heatpump to also take part in such reward schemes. It would need to get permission from some of its users to allow them to switch on or off heatpumps to be able to increase or reduce power consuption. It needs to combine several in order to reach the minimum power required to participate. This is 0.1MW. If a heatpump consumes 5KW then we talk about 20 heatpumops required to enable this. That seams feasible.
Sessy already has an arrangement with Frankenergy to realise profits from the "onbalans" market and for sure others will follow.

All these requirements require a control of the heatpump and at the moment that is only possible in a limited way by using the Weheat app.
It is possible there to adjust the power limit and silent hours for example. In addition you can adjust the control method. ie OpenTherm or "Stooklijn".
If we were able to use these setting we would gain some control of the heatpump and maybe in the future Weheat would support more of this functionality.
I am always open for any discussion on this topic with Weheat or anyone else.

As a first step i wanted to see if it was possible to have some interaction with the Weheat backoffice.
Via Mr Zeekoe who wrote a java application extracting logging data for display on Grafana i received help to make this possible.
The enclosed program is a proof of concept. It requests and access token and uses that to extract the most recent data from my heatpump.
There i can see the room temperature, room temperature setpoint and the different water temperatures.
This is done by using two API calls. The first one gets the access token which has been trunkated.
I do not know what the refresh_token nor the id_token are.<br/>
{<br/>
  "access_token": "eyJhbGciOiJSUzI1NiIsInR5cCIgOiA.............Ke5mYfCRTBoGLbEdc2JXXeceJV4kDg",<br/>
  "expires_in": 300,<br/>
  "refresh_expires_in": 2592000,<br/>
  "refresh_token": "eyJhbGciOiJIUzI1NiIsInR5cCIgOi.............v-sVBYWqiWSIG_2m1FatipZDLnCiNw",<br/>
  "token_type": "Bearer",<br/>
  "id_token": "eyJhbGciOiJSUzI1NiIsInR5cCIgOiAiSld.............XN9uolZEzyNX3dnfDqQjGjwhg4P0NQ",<br/>
  "not-before-policy": 1711716768,<br/>
  "session_state": "977622bb-49a6-4055-94af-a34bb0b9615a",<br/>
  "scope": "openid profile email"<br/>
}<br/>

The second call gets the actual data and looks like the following:<br/>
{<br/>
  "heatPumpId": "........",<br/>
  "timestamp": "2024-05-24T21:12:58.594Z",<br/>
  "tAirIn": 14.365114,<br/>
  "tAirOut": 14.730833,<br/>
  "tWaterIn": 18.202118,<br/>
  "tWaterOut": 18.077774,<br/>
  "tWaterHouseIn": 19.094078,<br/>
  "tRoom": 19.949219,<br/>
  "tRoomTarget": 17,<br/>
  "tThermostatSetpoint": 0,<br/>
  "otBoilerStatus": null,<br/>
  "otBoilerFeedTemperature": 20.5,<br/>
  "otBoilerReturnTemperature": 21,<br/>
  "interval": 5<br/>
}<br/>

Please note this is just a few of the items that are displayed. The actual list is much bigger.
Again the heatPumpId has been trunkated.

I hope this helps someone. I noticed that there are more components to the api.
It has for example a raw function and you are able to pass startTime and endTime.
This has not been included in the program.
