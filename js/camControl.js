const cameraButton = document.getElementById("sosButton");
const video = document.getElementById("cameraStream");
let stream;

cameraButton.addEventListener("click", async () => {
  try {
    if (!stream) {
      stream = await navigator.mediaDevices.getUserMedia({ video: true });
      video.srcObject = stream;
      cameraButton.textContent = "Turn Camera Off";
    } else {
      stream.getTracks().forEach((track) => track.stop());
      video.srcObject = null;
      stream = null;
      cameraButton.textContent = "Turn Camera On";
    }
  } catch (err) {
    console.error("Error accessing camera: ", err);
  }
});
