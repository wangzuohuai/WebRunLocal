<template>
	<div id="app">
		<router-view />
	</div>
</template>

<style>
	body {
		-webkit-font-smoothing: antialiased;
		padding: 0;
		margin: 0;
		font-family: 'PingFang SC';
		height: 100%;
		position: relative;

	}
	#app {
		height: 100%;		
		padding: 0;
		margin: 0;
	}
	.el-message-box__wrapper{
		z-index: 99999999!important;
	}
</style>
