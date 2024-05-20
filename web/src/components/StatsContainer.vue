<template>
    <div class="m-3 p-3">
        <h2>{{ title }}</h2>
        <div class="stats">
            <slot />
        </div>
    </div>
</template>

<script>
export default {
    name: 'StatsContainer',
    props: {
        title: String
    }
}
</script>
