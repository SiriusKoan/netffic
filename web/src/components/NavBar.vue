<template>
    <nav>
        <ul>
            <router-link to="/">Overview</router-link>
            <router-link to="/l3">L3</router-link>
            <router-link to="/l4">L4</router-link>
            <router-link to="/l7">L7</router-link>
        </ul>
    </nav>
</template>

<script>
export default {
    name: 'NavBar',
}
</script>
