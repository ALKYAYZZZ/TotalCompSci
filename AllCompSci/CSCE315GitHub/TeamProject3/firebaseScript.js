// Initialize Firebase
  var config = {
  apiKey: "AIzaSyDqTK6tM7VTcglEOZHgBuZEcLuPyCSpmWg",
  authDomain: "team-d7822.firebaseapp.com",
  databaseURL: "https://team-d7822.firebaseio.com",
  projectId: "team-d7822",
  storageBucket: "team-d7822.appspot.com",
  messagingSenderId: "778972024672"
  };
  firebase.initializeApp(config);

  //get elements
  const txtEmail = document.getElementById('txtEmail');
  const txtPassword = document.getElementById('txtPassword');
  const btnLogin = document.getElementById('btnLogin');
  const btnSignUp = document.getElementById('btnSignUp');
  const loginBtnTopBar = document.getElementById('loginButton');
  const logoutBtnTopBar = document.getElementById('logoutButton');
  const modal = document.getElementById('UserAuthentication');

  const email = txtEmail.value;
  const pass = txtPassword.value;
  const auth = firebase.auth();
  //add Login event
  btnLogin.addEventListener('click', e => {
    //Get email and pass
    const email = txtEmail.value;
    const pass = txtPassword.value;
    const auth = firebase.auth();
    //Sign in
    const promise = auth.signInWithEmailAndPassword(email, pass);
    promise.catch(function(e){
      alert(e.message);
    });
  });

  logoutBtnTopBar.addEventListener('click', e => {
    firebase.auth().signOut();
  });

  //Add signup event
  btnSignUp.addEventListener('click', e => {
  //Get email and pass
    const email = txtEmail.value;
    const pass = txtPassword.value;
    const auth = firebase.auth();

    //Sign in
    const promise = auth.createUserWithEmailAndPassword(email, pass);
    promise.catch(function(e){
      alert(e.message);
    });
  });

  //Add a realtime listener
  //Hide buttons that are not needed and show ones that are needed
  //based on whether the user is logged in or not
  firebase.auth().onAuthStateChanged(firebaseUser => {
  if(firebaseUser){
    btnLogin.style.display = "none";
    btnSignUp.style.display = "none";
    loginBtnTopBar.style.display = "none";
    logoutBtnTopBar.style.display = "block";
    modal.style.display = "none";
  }else {
      logoutBtnTopBar.style.display = "none";
      loginBtnTopBar.style.display = "block";
      if(btnLogin.style.display == "none"){
        btnLogin.style.display = "inline";
      }
      if(btnSignUp.style.display == "none"){
        btnSignUp.style.display = "inline";
      }
      modal.style.display = "none";
    }
  });
