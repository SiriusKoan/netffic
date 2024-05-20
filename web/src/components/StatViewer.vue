<template>
    <div class="stat">
        <div class="stat-title">{{ title }}</div>
        <div class="stat-value">{{ value }}</div>
        <div class="stat-desc">{{ desc }}</div>
    </div>
</template>

<script>
export default {
    name: 'StatViewer',
    props: {
        title: String,
        value: Number,
        desc: String
    }
}
</script>