function change()
{
    var box = document.getElementById("box")
    box.style.backgroundColor = `rgb(${r()}, ${r()}, ${r()})`;
}

const r = () => 0 + Math.floor(Math.random() * (255 - 0 + 1));