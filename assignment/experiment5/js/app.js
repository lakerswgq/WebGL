;(function(){


var renderer = null,
	scene = null,
	camera = null,
	controls = null,
	earth = null,
	earthScollDistance = 0.5,
	earthScrollSpeed = -0.02,
	fish,
	alien,
	alienShadow,
	clock = new THREE.Clock(  );
	// cube = null;

function TextureAnimator(texture, tilesHoriz, tilesVert, numTiles, tileDispDuration) 
{	
	// note: texture passed by reference, will be updated by the update function.
		
	this.tilesHorizontal = tilesHoriz;
	this.tilesVertical = tilesVert;
	// how many images does this spritesheet contain?
	//  usually equals tilesHoriz * tilesVert, but not necessarily,
	//  if there at blank tiles at the bottom of the spritesheet. 
	this.numberOfTiles = numTiles;
	texture.wrapS = texture.wrapT = THREE.RepeatWrapping; 
	texture.repeat.set( 1 / this.tilesHorizontal, 1 / this.tilesVertical );

	// how long should each image be displayed?
	this.tileDisplayDuration = tileDispDuration;

	// how long has the current image been displayed?
	this.currentDisplayTime = 0;

	// which image is currently being displayed?
	this.currentTile = 0;
		
	this.update = function( milliSec )
	{
		this.currentDisplayTime += milliSec;
		while (this.currentDisplayTime > this.tileDisplayDuration)
		{
			this.currentDisplayTime -= this.tileDisplayDuration;
			this.currentTile++;
			if (this.currentTile == this.numberOfTiles)
				this.currentTile = 0;
			var currentColumn = this.currentTile % this.tilesHorizontal;
			texture.offset.x = currentColumn / this.tilesHorizontal;
			var currentRow = Math.floor( this.currentTile / this.tilesHorizontal );
			texture.offset.y = currentRow / this.tilesVertical;
		}
	};
}	

var animate = function (){
	render();		
	update();
	requestAnimationFrame( animate );
}

var update = function (){
	var delta = clock.getDelta();
	fish.update( 1000 * delta );
}

var render = function (){
	if (earthScollDistance <= 0){
		earthScrollSpeed = 0.02;
		// return;
	}
	else if (earthScollDistance >= 0.5){
		earthScrollSpeed = -0.02;
	}
	earthScollDistance = (earthScrollSpeed > 0) ? earthScollDistance + 0.002 : earthScollDistance-0.002;
	earth.rotation.x += earthScrollSpeed;
	earth.position.z += earthScrollSpeed;
	// earth.position.x -= 0.01;

	renderer.render(scene, camera);
}

var createFish = function (scene){
	var geometry = THREE.cubeGeometry(0.7, 0.7, 0.7);

}

var loadJson = function (scene){
	var loader = new THREE.JSONLoader();
	var jsonUrl = "./Alien/walkingCycleMedium.js";
	loader.load(jsonUrl,function (data){
		var geometry = data,
			scale = 0.15;
		
		var alienRotation = 3;
		// Set up a morphing mesh and material
		var material = geometry.materials[ 0 ];
		material.morphTargets = true;
		material.color.setHex( 0xffFFFF ); // brighten it up
		material.ambient.setHex( 0xFFFFFF );
		var faceMaterial = new THREE.MeshFaceMaterial();
		
		alien = new THREE.MorphAnimMesh( geometry, faceMaterial  );
		alien.duration = 1000; // 1 second walk cycle
		alien.updateMatrix();
		alien.scale.set(scale, scale, scale);
		alien.rotation.y = alienRotation;
		scene.add( alien );
		// this.bodyMesh = mesh;
		
		// Fun! Fake shadow using a scaled version of the original mesh
		// plus shadowy material
		var material = new THREE.MeshBasicMaterial( 
				{ color: 0x444444, opacity: 0.8, morphTargets: true } );
		material.shading = THREE.FlatShading;
		alienShadow = new THREE.MorphAnimMesh( geometry, material  );
		alienShadow.scale.set(1, 0.001, 1.5);
		alienShadow.duration = 1000;
		alienShadow.scale.set(scale, scale, scale);
		// scene.add( alienShadow );	
		// this.shadowMesh = mesh2;
	});
}

var keyMove = function( event ){
	var speed = 0.05,
		key = event.keyCode,
		cameraPosition = camera.position,
		cameraRotation = camera.rotation,
		alienPosition = alien.position,
		alienRotation = alien.rotation;
	// console.log(key);
	if ( key === 87 ){ 	// key W
		cameraPosition.z -= speed;
		alienPosition.z -= speed;
	}
	else if ( key === 83 ){ 	// key S
		cameraPosition.z += speed;
		alienPosition.z += speed;
	}
	else if ( key === 65 ){	// key A
		cameraPosition.x -= speed;
		alienPosition.x -= speed;
	}
	else if ( key === 68 ){	// key D
		cameraPosition.x += speed;
		alienPosition.x += speed;
	}	
	else if ( key === 38 ){	// key Page UP
		cameraPosition.y += speed;
		alienPosition.y += speed;
	}
	else if ( key === 40 ){	//key Page Down
		cameraPosition.y -= speed;
		alienPosition.y -= speed;
	}
	else if ( key === 37 ){	// key Page Left 
		camera.rotation.y += speed;
		alienRotation.y += speed;
	}
	else if ( key ===39 ){	// key  Page Right
		camera.rotation.y -= speed;
		alienRotation.y -= speed;
	}
}

var createEarth = function ( scene ){
	var geometry = new THREE.SphereGeometry(0.8,16,16);
	var mapUrl = "./img/earth.jpg";
	var texture = THREE.ImageUtils.loadTexture(mapUrl);
	var material = new THREE.MeshPhongMaterial({ map: texture });
	earth = new THREE.Mesh(geometry, material);
	earth.position.set(-1.5,1,-3);
	earth.rotation.z = 0.41;
	earth.position.y += 0.01;
	scene.add(earth);

}

var init = function (){
	var container = document.getElementById("container");
	renderer = new THREE.WebGLRenderer(  {antialias:true}  );
	// renderer.setClearColor( 0xffffff, 1);
	var containerSize = container.getBoundingClientRect();
	renderer.setSize(containerSize.width, containerSize.height);
	container.appendChild(renderer.domElement);
	document.body.addEventListener("keydown", keyMove);

	scene = new THREE.Scene();

	camera = new THREE.PerspectiveCamera( 45, containerSize.width/containerSize.height, 1, 4000 );
	camera.position.set(1, 1.2, 3);
	// camera.lookAt (new THREE.Vector3 (0.0, 1.0, 0.0));
	// camera.lookAt(scene.position);
	scene.add( camera );	
}

var addLight = function (scene){
	var topLight = new THREE.PointLight(0xffffff);
	topLight.position.set(0,10,10);
	scene.add(topLight);

	// var bottomLight = new THREE.PointLight(0xffffff);
	// bottomLight.position.set(0,-250,0);
	// scene.add(bottomLight);
}

var createObject = function(scene){
	// 立方体
	var mapUrl = "./img/1.bmp";
	var map = THREE.ImageUtils.loadTexture(mapUrl);
	var material = new THREE.MeshPhongMaterial( {map: map} );
	var geometry = new THREE.CubeGeometry(0.7,0.7,0.7);
	var cube = new THREE.Mesh(geometry, material);
	cube.position.set(2.5,1,0);
	// cube.rotation.x = Math.PI/5;
	cube.rotation.y = Math.PI/5;
	scene.add(cube);

	createEarth(scene);

	var fishTexture = new THREE.ImageUtils.loadTexture( './img/fish.jpg' );
	fish = new TextureAnimator( fishTexture, 5, 5, 25, 55 ); // texture, #horiz, #vert, #total, duration.
	var fishMaterial = new THREE.MeshBasicMaterial( { map: fishTexture } );
	var cubeGeometry = new THREE.CubeGeometry( 0.5, 0.5, 0.5 );
	cube = new THREE.Mesh( cubeGeometry, fishMaterial );
	cube.position.set(1,0.8,0);
	scene.add(cube);

	// floor

	var floorTexture = new THREE.ImageUtils.loadTexture( './img/checker.png' );
	floorTexture.wrapS = floorTexture.wrapT = THREE.RepeatWrapping; 
	floorTexture.repeat.set( 10, 10 );
	var floorMaterial = new THREE.MeshBasicMaterial( { map: floorTexture, side: THREE.DoubleSide } );
	var floorGeometry = new THREE.PlaneGeometry(66, 66, 66, 66);
	var floor = new THREE.Mesh(floorGeometry, floorMaterial);
	floor.position.y = -0.01;
	floor.rotation.x = -Math.PI / 2;
	scene.add(floor);

	// var geometry = new THREE.PlaneGeometry(66, 66, 66, 66);
	// var map = THREE.ImageUtils.loadTexture('./img/checker.png');
 //    	map.repeat.set(11,11);
 //    	map.wrapS = map.wrapT = THREE.MirroredRepeatWrapping;
 //    	var floorMaterial = new THREE.MeshBasicMaterial({ map : map});
 //    	floorMaterial.side = THREE.BackSide;
	// var floor = new THREE.Mesh(geometry, floorMaterial);
	// floor.rotation.x = -Math.PI / 2;
	// floor.position.y = -.01;
	// scene.add(floor);
}

	init();

	addLight(scene);

	loadJson(scene);

	createObject(scene);

	animate();
})();