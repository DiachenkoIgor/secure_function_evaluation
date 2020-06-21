var calculateValueJS = Module.cwrap('calculateValueJS', 'string',['string','number','string','string','string']);

var globalWebAssemblyPerformance = 0;

function evaluateByte(array){
	var t0 = performance.now();
	for(var i = 0; i < array.length; i++){
		console.log(array[i]);
		calculateValue(array[i]);
	}
	var t1 = performance.now();
	console.log("All time - " + (t1 - t0));
	console.log("globalWebAssemblyPerformance   -  " + globalWebAssemblyPerformance);
    globalWebAssemblyPerformance = 0;
    
	return getRequest("/result",false);
}

function calculateValue(compareValue){
	var jsAnswer;
	var valueResult;
	var t0;
	var t1;
	//garbled
	var garbledTableJSON = getRequest("/garbled").jsonMessage;

	t0 = performance.now();
	valueResult = calculateValueJS("garbled", compareValue, garbledTableJSON, "", "");
	t1 = performance.now();
	globalWebAssemblyPerformance+= (t1-t0);


	jsAnswer = JSON.parse(valueResult);

	if(jsAnswer.execute != 0){
		alert("Error 'calculateValueJS'  -  "+jsAnswer.execute);
	}


	//oblivious_start and oblivious_end
	var startMessageJson= getRequest("/oblivious_start").jsonMessage;
	t0 = performance.now();
	valueResult = calculateValueJS("oblivious_start", compareValue, "", startMessageJson, "");
	t1 = performance.now();
	globalWebAssemblyPerformance+= (t1-t0);

	jsAnswer = JSON.parse(valueResult);

	if(jsAnswer.execute != 0){
		alert("Error 'calculateValueJS'  -  "+jsAnswer.execute);
	}

	postRequest("/oblivious_end", jsAnswer.jsonMessage);


	//oblivious_end and end
	var endMessageJson = getRequest("/oblivious_end").jsonMessage;
	t0 = performance.now();
	valueResult = calculateValueJS("end", compareValue, "", "", endMessageJson);
	t1 = performance.now();
	globalWebAssemblyPerformance+= (t1-t0);

	jsAnswer = JSON.parse(valueResult);

	if(jsAnswer.execute != 0){
		alert("Error 'calculateValueJS'  -  "+jsAnswer.execute);
	}

	postRequest("/end", jsAnswer.jsonMessage);
}

function getRequest(url, isJson = true){

	let xhr = new XMLHttpRequest();

	xhr.open('GET', url, false);
    
	try {
  		xhr.send();
  		if (xhr.status != 200) {
    		alert(`Ошибка ${xhr.status}: ${xhr.statusText}`);
   			return;
  		} else {
  			if(isJson){
                var t = xhr.response;
  				return JSON.parse(t);
  			}
    		return xhr.response;
  		}
	} catch(err) { 
  		alert("Запрос не удался");
	}
}

function postRequest(url, bodyJson){

	let xhr = new XMLHttpRequest();

	xhr.open("POST", url, false);
	xhr.setRequestHeader('Content-type', 'application/json; charset=utf-8');

	xhr.send(bodyJson);

}

function test(){
	var arr = [1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1];
	alert(evaluateByte(arr));
}


