const sosButton = document.getElementById('sosButton');
const circles = document.querySelectorAll('.circle');

function pointArrow(arrow, direction) {
    let rotation = 0;
    switch (direction) {
        case 'right':
            rotation = 0;
            break;
        case 'down':
            rotation = 90;
            break;
        case 'left':
            rotation = 180;
            break;
        case 'up':
            rotation = -90;
            break;
    }
    arrow.style.transform = `rotate(${rotation}deg)`;
}

function calculateDirection(deltaX, deltaY) {
    const angle = Math.atan2(deltaY, deltaX) * (180 / Math.PI);
    if (angle >= -45 && angle < 45) {
        return 'right';
    } else if (angle >= 45 && angle < 135) {
        return 'down';
    } else if (angle >= -135 && angle < -45) {
        return 'up';
    } else {
        return 'left';
    }
}

function updateArrows() {
    const targetRect = sosButton.getBoundingClientRect();
    const targetX = targetRect.left + targetRect.width / 2;
    const targetY = targetRect.top + targetRect.height / 2;

    circles.forEach(c => {
        const rect = c.getBoundingClientRect();
        const deltaX = targetX - (rect.left + rect.width / 2);
        const deltaY = targetY - (rect.top + rect.height / 2);

        const direction = calculateDirection(deltaX, deltaY);
        const arrow = c.querySelector('.arrow');
        pointArrow(arrow, direction);
    });
}

// Make the SOS button draggable
sosButton.addEventListener('mousedown', function (e) {
    e.preventDefault();
    let shiftX = e.clientX - sosButton.getBoundingClientRect().left;
    let shiftY = e.clientY - sosButton.getBoundingClientRect().top;

    function moveAt(pageX, pageY) {
        sosButton.style.left = pageX - shiftX + 'px';
        sosButton.style.top = pageY - shiftY + 'px';
    }

    function onMouseMove(e) {
        moveAt(e.pageX, e.pageY);
    }

    document.addEventListener('mousemove', onMouseMove);

    sosButton.addEventListener('mouseup', function onMouseUp() {
        document.removeEventListener('mousemove', onMouseMove);
        updateArrows(); // Ensure arrows point correctly after dragging
        sosButton.removeEventListener('mouseup', onMouseUp); // Clean up the event listener
    });
});

sosButton.ondragstart = function () {
    return false;
};

sosButton.style.position = 'absolute';
sosButton.style.left = '50%';
sosButton.style.top = '26%';

// Ensure arrows are correctly updated on both click and after dragging
sosButton.addEventListener('click', 
    () => {
        updateArrows(); // Ensure arrows point correctly after dragging
        
         sosButton.classList.toggle('active');
    }
);
