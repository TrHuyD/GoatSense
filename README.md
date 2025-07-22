
<a id="readme-top"></a>
<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/TrHuyD/iChat">
    <img src="https://img.freepik.com/premium-vector/farmer-feeding-farm-animals-flat-cartoon-vector-illustration_199064-2483.jpg?semt=ais_items_boosted&w=740" alt="Logo" width="80" height="80">
  </a>
    <p align="center">
  <h3 align="center">GoatSense</h3>
 Smart Environmental Monitoring for Goat Farms.
    <br />
      <br />
  </div>




<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#features">Features</a></li>
       <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li><a href="#roadmap">Roadmap</a></li>
  </ol>
</details>

<!--
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
-->

<!-- ABOUT THE PROJECT -->
## About The Project

![Product Name Screen Shot](https://github.com/TrHuyD/GoatSense/blob/main/images/demo.png?raw=true)
A real-time IoT-based system leveraging deep learning to monitor and predict environmental conditions in goat farms. This system helps ensure animal well-being by detecting harmful gas levels and controlling devices like fans and sprayers automatically.
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Features


* Predictive monitoring of:
    * Temperature (°C)
    * Humidity (%)
    * Ammonia (NH₃, ppm)
    * Hydrogen Sulfide (H₂S, ppm)
* Real-time data collection and visualization via ThingsBoard
* Time-series prediction using LSTM deep learning models
* Automated environmental response: fan and pump activation on threshold breach
* Runs directly on Raspberry Pi 3


### Built With


* ![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge\&logo=python\&logoColor=white)
* ![TensorFlow Lite](https://img.shields.io/badge/TensorFlow%20Lite-FF6F00?style=for-the-badge\&logo=tensorflow\&logoColor=white)
* ![ESP32](https://img.shields.io/badge/ESP32-005BBB?style=for-the-badge\&logo=espressif\&logoColor=white)
* ![Raspberry Pi](https://img.shields.io/badge/Raspberry%20Pi-C51A4A?style=for-the-badge\&logo=raspberry-pi\&logoColor=white)
* ![ThingsBoard](https://img.shields.io/badge/ThingsBoard-2088FF?style=for-the-badge\&logo=iot\&logoColor=white)
* ![WANDB](https://img.shields.io/badge/W%26B-FFBE00?style=for-the-badge\&logo=weights-and-biases\&logoColor=black)


<p align="right">(<a href="#readme-top">back to top</a>)</p>




## Getting Started
<!-- GETTING STARTED 
This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.



This is an example of how to list things you need to use the software and how to install them.
* npm
  ```sh
  npm install npm@latest -g
  ```
  -->
  ### Prerequisites
### Installation
  <!--
_Below is an example of how you can instruct your audience on installing and setting up your app. This template doesn't rely on any external dependencies or services._

1. Get a free API Key at [https://example.com](https://example.com)
2. Clone the repo
   ```sh
   git clone https://github.com/github_username/repo_name.git
   ```
3. Install NPM packages
   ```sh
   npm install
   ```
4. Enter your API in `config.js`
   ```js
   const API_KEY = 'ENTER YOUR API';
   ```
5. Change git remote url to avoid accidental pushes to base project
   ```sh
   git remote set-url origin github_username/repo_name
   git remote -v # confirm the changes
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

-->


## Usage


- Temperature – Predicted vs. Actual

    <img src="https://github.com/TrHuyD/GoatSense/blob/main/images/temp.png?raw=true" width="400" />

- Mois – Predicted vs. Actual

    <img src="https://github.com/TrHuyD/GoatSense/blob/main/images/mois.png?raw=true" width="400" />


<p align="right">(<a href="#readme-top">back to top</a>)</p>




<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: images/screenshot.png
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
