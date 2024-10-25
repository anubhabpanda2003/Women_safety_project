const siren = new Audio("assets/alarm.mp3"); // Change to a valid audio file
siren.loop = true;
let isSirenPlaying = false; // Flag to track the siren state

document.querySelectorAll(".sosButton").forEach((button) => {
  button.addEventListener("click", function () {
    const vignette = document.querySelector(".blinking-lights");

    if (isSirenPlaying) {
      // Stop siren and animation when SOS button is pressed again
      vignette.classList.add("hidden");
      vignette.style.animation = "";
      siren.pause();
      siren.currentTime = 0; // Reset to start
      isSirenPlaying = false;
    } else {
      // Remove the hidden class to reveal the animation
      vignette.classList.remove("hidden");

      // Apply animation and play siren
      vignette.style.animation = "blink 1s infinite"; // Use the existing blink animation
      siren.play();
      isSirenPlaying = true;
    }
  });

  // Add an event listener to the document to pause the siren and hide the animation when clicked outside the SOS button
  document.addEventListener("click", function (event) {
    if (!button.contains(event.target)) {
      const vignette = document.querySelector(".blinking-lights");

      // Add the hidden class to hide the animation
      vignette.classList.add("hidden");

      // Pause siren and reset animation
      vignette.style.animation = "";
      if (isSirenPlaying) {
        siren.pause();
        siren.currentTime = 0; // Reset to start
        isSirenPlaying = false;
      }
    }
  });
});
