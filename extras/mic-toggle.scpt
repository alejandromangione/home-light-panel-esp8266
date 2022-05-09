var app = Application.currentApplication();
app.includeStandardAdditions = true;

function disableMicrophone() {
 app.setVolume(null, { inputVolume: 0});
 app.displayNotification("Microphone Off");
}

function enableMicrophone() {
 app.setVolume(null, { inputVolume: 50});
 app.displayNotification("Microphone On");
}

function sendRequest(status) {
  app.doShellScript(`curl -X GET "http://192.168.1.170/mic?status=${status}"`);
}

function getMicrophoneVolume() {
  return Number(app.getVolumeSettings()['inputVolume']);
}

function run(input, parameters) {
  console.log('Init');

  if(getMicrophoneVolume() > 0) {
    disableMicrophone();
	sendRequest(0);
  } else {
    enableMicrophone();
	sendRequest(1);
  }

  return input;
}
