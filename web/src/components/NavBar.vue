<template>
    <div class="navbar bg-base-300">
        <div class="flex-1">
            <ul class="menu menu-horizontal px-1">
                <li><router-link to="/">Overview</router-link></li>
                <li><router-link to="/l3">L3</router-link></li>
                <li><router-link to="/l4">L4</router-link></li>
                <li><router-link to="/l7">L7</router-link></li>
            </ul>
        </div>
    </div>
</template>

<script>
export default {
    name: 'NavBar',
}
</script>
