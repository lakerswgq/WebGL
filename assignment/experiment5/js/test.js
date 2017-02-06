
		var rendeder=null,camera=null,cube=null,animating=false;
		window.onload=load;
		function load(){
			var container=document.getElementById("container");
			renderer=new THREE.WebGLRenderer( { antialias: true } );
			renderer.setSize(container.offsetWidth,container.offsetHeight);
			renderer.setClearColor( 0xffffff, 1);
			container.appendChild(renderer.domElement);
			scene=new THREE.Scene();
			camera=new THREE.PerspectiveCamera(40,container.offsetWidth/container.offsetHeight,1,100);
			camera.position.set(0,0,3);
			var light=new THREE.PointLight(0xfffff,1.5);
			light.position.set(0,0,1);
			scene.add(light);
			var mapUrl="img/3.jpg";
			var loader = new THREE.TextureLoader();
			var map = loader.load(mapUrl);
			// var map=new THREE.ImageUtils.loadTexture(mapUrl);
			var material=new THREE.MeshPhongMaterial( { map: map } );
			var geometry=new THREE.CubeGeometry(1,1,1);
			cube=new THREE.Mesh(geometry,material);
			cube.rotation.x=Math.PI/5;
			cube.rotation.y=Math.PI/5;
			// cube.rotation.z=Math.PI/5;
			scene.add(cube);
			addMouseHandler();
			run();
		}
		function run(){
			renderer.render(scene,camera);
			if(animating){
				cube.rotation.y -= 0.01;
			}
			requestAnimationFrame(run);
		}
		function addMouseHandler(){
			var dom=renderer.domElement;
			dom.addEventListener("mouseup", onMouseUp,false);
		}
		function onMouseUp(event){
			event.preventDefault();
			animating=!animating;
		}
	