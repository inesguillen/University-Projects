const expirationDate = 'December 24, 2024 23:59:59'; //FECHA LIMITE
    let deadlineTime = new Date(expirationDate);

    deadlineTime.setDate(deadlineTime.getDate());
    let deadline = deadlineTime.getTime(); //DIFERENCIA ENTRE FECHA ACTUAL Y FECHA LIMITE
    
    function updateCountdown() {
      if (countdownInterval !== null) {
        let ahora = new Date().getTime();    
        let restante = deadline - ahora;
      
        let dias = Math.floor(restante / (1000 * 60 * 60 * 24));
        let horas = Math.floor((restante % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
        let minutos = Math.floor((restante % (1000 * 60 * 60)) / (1000 * 60));
        let seg = Math.floor((restante % (1000 * 60)) / 1000);
      
        let diasElements = document.getElementsByClassName("dias");
        let horasElements = document.getElementsByClassName("horas");
        let minutosElements = document.getElementsByClassName("minutos");
        let segElements = document.getElementsByClassName("segundos");
    
        //ACTUALIZAMOS VALORES
        Array.from(diasElements).forEach(el => el.innerHTML = dias);
        Array.from(horasElements).forEach(el => el.innerHTML = horas);
        Array.from(minutosElements).forEach(el => el.innerHTML = minutos);
        Array.from(segElements).forEach(el => el.innerHTML = seg);
      
        //FECHA LIMITE YA HA PASADO
        if (restante < 0) {
          countdownInterval && clearInterval(countdownInterval); //LIMPIAMOS Y DETENEMOS CUENTA REGRESIVA
          Array.from(diasElements).forEach(el => el.innerHTML = 0);
          Array.from(horassElements).forEach(el => el.innerHTML = 0);
          Array.from(minutosElements).forEach(el => el.innerHTML = 0);
          Array.from(segElements).forEach(el => el.innerHTML = 0);
        }
      }
    }
    
    let countdownInterval = null;
    updateCountdown(); //LLAMAMOS LA INICIO PARA ACTUALIZAR CONTADOR
    countdownInterval = setInterval(updateCountdown, 1000);